#include "libtcod.hpp"
#include "game_map.hpp"
#include "engine.hpp"
#include "actors.hpp"

namespace rogue {

static int last_x1, last_y1, last_x2, last_y2;

static const int MAX_ROOM_SIZE = 12;
static const int MIN_ROOM_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;

class BSPListener : public ITCODBspCallback {
public:
    BSPListener(Map& map) : map_(map), room_num_(0) {};

    bool visitNode(TCODBsp* node, void *userData) override {
        if (node->isLeaf()) {
            TCODRandom* rng = TCODRandom::getInstance();
            int w = rng->getInt(MIN_ROOM_SIZE, node->w - 2);
            int h = rng->getInt(MIN_ROOM_SIZE, node->h - 2);
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
        return true;
    }

private:
    Map& map_;
    int room_num_;
    int last_x_;
    int last_y_;
};

Map::Map(int width, int height) : width(width), height(height), princess_placed_(false) {
    tiles_ = new Tile[width * height];
    map = new TCODMap(width, height);
    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, MIN_ROOM_SIZE, MAX_ROOM_SIZE, 1.5F, 1.5F);
    BSPListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
    PutPrincess(last_x1, last_y1, last_x2, last_y2);
}

Map::~Map() {
    delete[] tiles_;
    delete map;
}

bool Map::IsWall(int x, int y) {
    return !map->isWalkable(x, y);
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
                    IsWall(x, y) ? light_wall: light_ground);
            }
            else if (IsExplored(x, y)){
                TCODConsole::root->setCharBackground(x, y,
                    IsWall(x, y) ? wall : ground);
            }
        }
    }
}

bool Map::IsInFOV(int x, int y) {
    if (map->isInFov(x, y)) {
        tiles_[x + y * width].explored = true;
        return true;
    }
    return false;
}

bool Map::IsExplored(int x, int y) {
    return tiles_[x + y * width].explored;
}

void Map::ComputeFOV() {
    map->computeFov(engine.player->x, engine.player->y, engine.fov_radius);
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
            map->setProperties(x, y, true, true);
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

}