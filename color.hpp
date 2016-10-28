#pragma once
#include "libtcod.hpp"

namespace rogue {

class Color : public TCODColor {
public:
    Color(int c);
};

}