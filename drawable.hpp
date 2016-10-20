#pragma once
#include "libtcod.hpp"

namespace rogue {

class Drawable {
public:
    Drawable(int x, int y, int symbol, const TCODColor& color, const char* name);
    virtual ~Drawable() = 0;

    void Render(int mx, int my);

    int x, y;
    int symbol;
    TCODColor color;
    const char* name;
};

}