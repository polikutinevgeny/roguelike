#pragma once
#include "drawable.hpp"
#include <vector>

namespace rogue {

struct Tile {
    Tile() : explored(false) {};
    bool explored;
};

class Map {
public:
    Map(int width, int height);
    ~Map();

    bool IsWall(int x, int y);
    bool CanWalk(int x, int y);
    void Render();

    bool IsInFOV(int x, int y);
    bool IsExplored(int x, int y);
    void ComputeFOV();

    void AddMonster(int x, int y);

    int width, height;

protected:
    Tile *tiles_;
    friend class BSPListener;

    TCODMap* map;

    void Dig(int x1, int y1, int x2, int y2);
    void CreateRoom(bool first, int x1, int y1, int x2, int y2);
};

}