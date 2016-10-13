#pragma once
#include "actors.hpp"
#include <vector>

namespace rogue {

struct Tile {
    Tile() : explored(false), walkable(false), transparent(false), fov(false) {};
    bool explored;
    bool walkable;
    bool fov;
    bool transparent;
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
    Tile *map_;
    friend class BSPCallback;

    bool princess_placed_;

    void CastLight(int cx, int cy, int row, double start, double end, int radius, int r2,
        int xx, int xy, int yx, int yy);

    void Dig(int x1, int y1, int x2, int y2);
    void CreateRoom(bool first, int x1, int y1, int x2, int y2);

    void PutPrincess(int x1, int y1, int x2, int y2);

};

}