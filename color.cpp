#include "color.hpp"

rogue::Color::Color(int c) : TCODColor((c >> 16) & 255, (c >> 8) & 255, c & 255){}
