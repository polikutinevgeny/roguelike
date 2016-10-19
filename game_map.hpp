#pragma once
#include <vector>
#include "actors.hpp"

namespace rogue {

class MapCallbackInterface {
public:
    virtual Player* GetPlayer() = 0;
    virtual Actor* GetPrincess() = 0;
    virtual std::list<Actor*>& GetActors() = 0;
    virtual std::list<Potion*>& GetLoot() = 0;
    virtual int GetFOVRadius() = 0;
};

struct Tile {
    Tile() : explored(false), walkable(false), transparent(false), fov(false) {};
    bool explored;
    bool walkable;
    bool fov;
    bool transparent;
};

class Map {
public:
    Map(int width, int height, MapCallbackInterface& engine, ActorCallbackInterface& actor_callback);

    bool IsWall(int x, int y);
    bool CanWalk(int x, int y);
    void Render(int mx, int my);

    bool IsInFOV(int x, int y);
    bool IsExplored(int x, int y);
    bool IsInBounds(int x, int y);
    void ComputeFOV();

    void Destroy(int x, int y);

    int width, height;

private:
    MapCallbackInterface& engine_;
    ActorCallbackInterface& actor_callback_;

    std::vector<Tile> map_;
    friend class BSPListener;

    void CastLight(int cx, int cy, int row, double start, double end, 
        int radius, int xx, int xy, int yx, int yy);

    void Dig(int x1, int y1, int x2, int y2);
    void CreateRoom(bool first, int x1, int y1, int x2, int y2);

    void PutPrincess(int x1, int y1, int x2, int y2);
    void PutMonster(int x, int y);

};

}