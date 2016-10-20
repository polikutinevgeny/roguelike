#include "drawable.hpp"

namespace rogue {

Drawable::Drawable(int x, int y, int symbol, const TCODColor & color, const char * name)
    : x(x), y(y)
    , symbol(symbol)
    , color(color)
    , name(name) 
{}

Drawable::~Drawable() {}

void Drawable::Render(int mx, int my) {
    TCODConsole::root->setChar(x + mx, y + my, symbol);
    TCODConsole::root->setCharForeground(x + mx, y + my, color);
}

}