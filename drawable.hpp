#pragma once
#include "libtcod.hpp"

namespace rogue {

class Actor {
public:
    Actor(int x, int y, int face, const TCODColor& color);
    Actor(int x, int y, int face, const TCODColor& color, const char* name);

    void Render();

    void Update();

    bool MoveOrAttack(int x, int y);

    int x, y;
    int face;
    TCODColor color;
    const char* name;
};

}