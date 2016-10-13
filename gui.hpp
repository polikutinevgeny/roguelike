#pragma once
#include "libtcod.hpp"

namespace rogue {

class Gui {
public:
    Gui();
    ~Gui();

    void Render();

protected:
    TCODConsole* con_;

    void RenderBar(int x, int y, int width, const char* name, int value, int max_value, const TCODColor& bar_color, const TCODColor& back_color);
};

}