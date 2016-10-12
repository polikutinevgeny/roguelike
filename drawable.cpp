#include "drawable.hpp"
#include "engine.hpp"
#include "libtcod.hpp"
#include <stdio.h>

namespace rogue {



Actor::Actor(int x, int y, int face, const TCODColor & color) : 
    x(x), y(y), face(face), color(color) {}

Actor::Actor(int x, int y, int face, const TCODColor & color, const char * name) :
    x(x), y(y), face(face), color(color), name(name) {
}

void Actor::Render() {
    TCODConsole::root->setChar(x, y, face);
    TCODConsole::root->setCharForeground(x, y, color);
}

void Actor::Update() {
    //printf("The %s says hello.\n", name);
}

bool Actor::MoveOrAttack(int x, int y) {
    if (engine.map->IsWall(x, y)) {
        return false;
    }
    for (auto a : engine.actors) {
        if (a->x == x && a->y == y) {
            printf("%s pleads not to kill him.\n", a->name);
            return false;
        }
    }
    this->x = x;
    this->y = y;
    return true;
}

}