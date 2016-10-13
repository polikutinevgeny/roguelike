#include "libtcod.hpp"
#include "game_map.hpp"
#include "engine.hpp"
#include "actors.hpp"
#include "bsp_tree.hpp"

namespace rogue {

static int last_x1, last_y1, last_x2, last_y2;

static const int MAX_ROOM_MONSTERS = 3;
static const int MIN_ROOM_VERT_SIZE = 12;
static const int MIN_ROOM_HORIZ_SIZE = 6;

class BSPListener : public BSPTreeCallbackInterface {
public:
    BSPListener(Map& map) : map_(map), room_num_(0) {};

    virtual void VisitNode(BSPTree const* node) override {
        if (node->IsLeaf()) {
            TCODRandom* rng = TCODRandom::getInstance();
            int w = rng->getInt(MIN_ROOM_HORIZ_SIZE, node->w - 2);
            int h = rng->getInt(MIN_ROOM_VERT_SIZE, node->h - 2);
            int x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
            int y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
            map_.CreateRoom(room_num_ == 0, x, y, x + w - 1, y + h - 1);
            if (room_num_ != 0) {
                map_.Dig(last_x_, last_y_, x + w / 2, last_y_);
                map_.Dig(x + w / 2, last_y_, x + w / 2, y + h / 2);
            }
            last_x_ = x + w / 2;
            last_y_ = y + h / 2;
            room_num_++;
        }
    }
private:
    Map& map_;
    int room_num_;
    int last_x_;
    int last_y_;
};

static int mult[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1},
};

Map::Map(int width, int height) : width(width), height(height), princess_placed_(false) {
    map_ = new Tile[width * height];
    BSPTree bsp(1, 1, width - 3, height - 3);
    bsp.SplitRecursive(8, MIN_ROOM_HORIZ_SIZE, MIN_ROOM_VERT_SIZE, 1.5, 1.5);
    BSPListener callback(*this);
    bsp.TraverseLevelOrder(&callback);
}

Map::~Map() {
    delete[] map_;
}

bool Map::IsWall(int x, int y) {
    return x < 0 || y < 0 || x >= width || y >= height || !map_[x + y * width].walkable;
}

bool Map::CanWalk(int x, int y) {
    if (IsWall(x, y)) {
        return false;
    }
    for (auto a : engine.actors) {
        if (a->x == x && a->y == y && a->blocks) {
            return false;
        }
    }
    return true;
}

void Map::Render() {
    static const TCODColor wall(0, 0, 100);
    static const TCODColor ground(50, 50, 150);
    static const TCODColor light_wall(130, 110, 50);
    static const TCODColor light_ground(200, 180, 50);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (IsInFOV(x, y)) {
                TCODConsole::root->setCharBackground(x, y,
                    IsWall(x, y) ? light_wall : light_ground);
            }
            else if (IsExplored(x, y)) {
                TCODConsole::root->setCharBackground(x, y,
                    IsWall(x, y) ? wall : ground);
            }
        }
    }
}

bool Map::IsInFOV(int x, int y) {
    if (map_[x + y * width].fov) {
        map_[x + y * width].explored = true;
        return true;
    }
    return false;
}

bool Map::IsExplored(int x, int y) {
    return map_[x + y * width].explored;
}

void Map::ComputeFOV() {
    for (int i = 0; i < width * height; ++i) {
        map_[i].fov = false;
    }
    for (int oct = 0; oct < 8; ++oct) {
        CastLight(
            engine.player->x, engine.player->y, 1, 1.0, 0.0, engine.fov_radius, 
            engine.fov_radius * engine.fov_radius, mult[0][oct], mult[1][oct], mult[2][oct], mult[3][oct]);
    }
    map_[engine.player->x + engine.player->y * width].fov = 1;
}

void Map::AddMonster(int x, int y) {
    TCODRandom* rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 80) {
        engine.actors.push_back(new Zombie(x, y, 'Z', TCODColor::desaturatedGreen, "zombie"));
    }
    else {
        engine.actors.push_back(new Dragon(x, y, 'D', TCODColor::red, "dragon"));
    }
}

void Map::Dig(int x1, int y1, int x2, int y2) {
    if (x2 < x1) {
        std::swap(x2, x1);
    }
    if (y2 < y1) {
        std::swap(y2, y1);
    }
    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            map_[x + y * width].transparent = true;
            map_[x + y * width].walkable = true;
        }
    }
}

void Map::CreateRoom(bool first, int x1, int y1, int x2, int y2) {
    Dig(x1, y1, x2, y2);
    last_x1 = x1;
    last_x2 = x2;
    last_y1 = y1;
    last_y2 = y2;
    if (first) {
        engine.player->x = (x1 + x2) / 2;
        engine.player->y = (y1 + y2) / 2;
    }
    else {
        TCODRandom* rng = TCODRandom::getInstance();
        int num = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (num > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (CanWalk(x, y)) {
                AddMonster(x, y);
            }
            num--;
        }
    }
}

void Map::PutPrincess(int x1, int y1, int x2, int y2) {
    if (x2 < x1) {
        std::swap(x2, x1);
    }
    if (y2 < y1) {
        std::swap(y2, y1);
    }
    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            if (CanWalk(x, y)) {
                engine.princess->x = x;
                engine.princess->y = y;
                return;
            }
        }
    }
}

void Map::CastLight(int cx, int cy, int row, double start, double end, int radius, int r2,
    int xx, int xy, int yx, int yy) {
    double new_start = 0.0f;
    if (start < end) {
        return;
    }
    for (int j = row; j < radius + 1; j++) {
        int dx = -j - 1;
        int dy = -j;
        bool blocked = false;
        while (dx <= 0) {
            dx++;
            int x = cx + dx * xx + dy * xy;
            int y = cy + dx * yx + dy * yy;
            if (x < width && y < height && x >= 0 && y >= 0) {
                int offset = x + y * width;
                double l_slope = (dx - 0.5) / (dy + 0.5);
                double r_slope = (dx + 0.5) / (dy - 0.5);
                if (start < r_slope) {
                    continue;
                }
                else if (end > l_slope) {
                    break;
                }
                if (dx * dx + dy * dy <= r2) {
                    map_[offset].fov = 1;
                }
                if (blocked) {
                    if (!map_[offset].transparent) {
                        new_start = r_slope;
                        continue;
                    }
                    else {
                        blocked = false;
                        start = new_start;
                    }
                }
                else {
                    if (!map_[offset].transparent && j < radius) {
                        blocked = true;
                        CastLight(cx, cy, j + 1, start, l_slope, radius, r2, xx, xy, yx, yy);
                        new_start = r_slope;
                    }
                }
            }
        }
        if (blocked) {
            break;
        }
    }
}

}