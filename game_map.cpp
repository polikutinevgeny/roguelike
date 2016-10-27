#include "libtcod.hpp"
#include "game_map.hpp"
#include "bsp_tree.hpp"
//#include "loot.hpp"
#include "config.hpp"

namespace rogue {

namespace {

int last_x1, last_y1, last_x2, last_y2;

const int MAX_ROOM_MONSTERS = GetConfigValue("MAX_ROOM_MONSTERS");
const int MIN_ROOM_VERT_SIZE = GetConfigValue("MIN_ROOM_VERT_SIZE");
const int MIN_ROOM_HORIZ_SIZE = GetConfigValue("MIN_ROOM_HORIZ_SIZE");

}

class BSPListener : public BSPTreeCallbackInterface {
public:
    BSPListener(Map& map) 
        : map_(map), room_num_(0) {};

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

Map::Map(int width, int height, MapCallbackInterface& engine, ActorCallbackInterface& actor_callback) 
    : width(width), height(height), engine_(engine), actor_callback_(actor_callback) 
{
    map_.resize(width * height);
    BSPTree bsp(1, 1, width - 2, height - 2);
    bsp.SplitRecursive(8, MIN_ROOM_HORIZ_SIZE, MIN_ROOM_VERT_SIZE, 1.5, 1.5);
    BSPListener callback(*this);
    bsp.TraverseLevelOrder(callback);
    PutPrincess(last_x1, last_y1, last_x2, last_y2);
}

bool Map::IsWall(int x, int y) {
    return x < 0 || y < 0 || x >= width || y >= height || !map_[x + y * width].walkable;
}

bool Map::CanWalk(int x, int y) {
    if (IsWall(x, y)) {
        return false;
    }
    for (auto a : engine_.GetActors()) {
        if (a->x == x && a->y == y && a->blocks) {
            return false;
        }
    }
    return true;
}

void Map::Render(int mx, int my) {
    static const TCODColor wall(0, 0, 100);
    static const TCODColor ground(50, 50, 150);
    static const TCODColor light_wall(130, 110, 50);
    static const TCODColor light_ground(200, 180, 50);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (IsInFOV(x, y)) {
                TCODConsole::root->setCharBackground(x + mx, y + my,
                    IsWall(x, y) ? light_wall : light_ground);
            }
            else if (IsExplored(x, y)) {
                TCODConsole::root->setCharBackground(x + mx, y + my,
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

bool Map::IsInBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height;
}

void Map::ComputeFOV() {
    static int mult[4][8] = {
        { 1, 0, 0, -1, -1, 0, 0, 1 },
        { 0, 1, -1, 0, 0, -1, 1, 0 },
        { 0, 1, 1, 0, 0, -1, -1, 0 },
        { 1, 0, 0, 1, -1, 0, 0, -1 },
    };
    for (int i = 0; i < width * height; ++i) {
        map_[i].fov = false;
    }
    for (int oct = 0; oct < 8; ++oct) {
        CastLight(
            engine_.GetPlayer()->x, engine_.GetPlayer()->y, 1, 1.0, 0.0, engine_.GetFOVRadius(), 
            mult[0][oct], mult[1][oct], mult[2][oct], mult[3][oct]);
    }
    map_[engine_.GetPlayer()->x + engine_.GetPlayer()->y * width].fov = 1;
}

void Map::OpenMap() {
    for (int i = 0; i < width * height; ++i) {
        map_[i].explored = true;
    }
}

void Map::Destroy(int x, int y) {
    map_[x + width * y].walkable = true;
    map_[x + width * y].transparent = true;
    ComputeFOV();
}

void Map::PutMonster(int x, int y) {
    TCODRandom* rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 80) {
        engine_.GetActors().push_back(new Zombie(
            x, y, 'Z', TCODColor::darkerGreen, "zombie", actor_callback_));
    }
    else {
        engine_.GetActors().push_back(new Dragon(
            x, y, 'D', TCODColor::red, "dragon", actor_callback_));
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
        engine_.GetPlayer()->x = (x1 + x2) / 2;
        engine_.GetPlayer()->y = (y1 + y2) / 2;
    }
    else {
        TCODRandom* rng = TCODRandom::getInstance();
        int num = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (num > 0) {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (CanWalk(x, y)) {
                PutMonster(x, y);
            }
            num--;
        }
        /*int x = rng->getInt(x1, x2);
        int y = rng->getInt(y1, y2);
        if (CanWalk(x, y)) {
            engine_.GetLoot().push_back(CreatePotion(x, y, engine_.GetPlayer()));
        }*/
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
                engine_.GetPrincess()->x = x;
                engine_.GetPrincess()->y = y;
                return;
            }
        }
    }
}

void Map::CastLight(int cx, int cy, int row, double start, double end, 
    int radius, int xx, int xy, int yx, int yy) {
    double new_start = 0.0;
    if (start < end) {
        return;
    }
    for (int j = row; j < radius + 1; j++) {
        bool blocked = false;
        for (int dx = -j, dy = -j; dx <= 0; ++dx) {
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
                if (dx * dx + dy * dy <= radius * radius) {
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
                        CastLight(cx, cy, j + 1, start, l_slope, radius, xx, xy, yx, yy);
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