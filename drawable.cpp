#include "drawable.hpp"
#include "libtcod.hpp"

namespace rogue {

void Knight::Draw() {
    TCODConsole::root->putChar(x_, y_, 'K');
}

void Knight::Move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}

void Knight::Hit() {
}

CollisionType Knight::CheckCollision (Drawable & other) {
    return other.CheckCollision(*this);
}

CollisionType Knight::CheckCollision (Knight & other) {
    return ERROR_COLLISION;
}

CollisionType Knight::CheckCollision (Princess & other) {
    return PRINCESS_COLLISION;
}

CollisionType Knight::CheckCollision (Monster & other) {
    return FIGHT_COLLISION;
}

CollisionType Knight::CheckCollision (Wall & other) {
    return OBJECT_COLLISION;
}

CollisionType Knight::CheckCollision (Floor & other) {
    return NO_COLLISION;
}

void Princess::Draw() {
    TCODConsole::root->putChar(x_, y_, 'P');
}

void Princess::Move(int dx, int dy) {
    //Princess could move. In the future. Maybe.
}

CollisionType Princess::CheckCollision (Drawable & other) {
    return other.CheckCollision(*this);
}

CollisionType Princess::CheckCollision (Knight & other) {
    return PRINCESS_COLLISION;
}

CollisionType Princess::CheckCollision (Princess & other) {
    return ERROR_COLLISION;
}

CollisionType Princess::CheckCollision (Monster & other) {
    return NO_FIGHT_COLLISION;
}

CollisionType Princess::CheckCollision (Wall & other) {
    return OBJECT_COLLISION;
}

CollisionType Princess::CheckCollision (Floor & other) {
    return NO_COLLISION;
}

void Zombie::Draw() {
    TCODConsole::root->putChar(x_, y_, 'Z');
}

void Zombie::Move(int dx, int dy) {
}

void Zombie::Hit() {
}

void Dragon::Draw() {
    TCODConsole::root->putChar(x_, y_, 'D');
}

void Dragon::Move(int dx, int dy) {
}

void Dragon::Hit() {
}

void Wall::Draw() {
    TCODConsole::root->putChar(x_, y_, '#');
}

CollisionType Wall::CheckCollision (Drawable & other) {
    return other.CheckCollision(*this);
}

CollisionType Wall::CheckCollision (Knight & other) {
    return OBJECT_COLLISION;
}

CollisionType Wall::CheckCollision (Princess & other) {
    return OBJECT_COLLISION;
}

CollisionType Wall::CheckCollision (Monster & other) {
    return OBJECT_COLLISION;
}

CollisionType Wall::CheckCollision (Wall & other) {
    return ERROR_COLLISION;
}

CollisionType Wall::CheckCollision (Floor & other) {
    return ERROR_COLLISION;
}

void Floor::Draw() {
    TCODConsole::root->putChar(x_, y_, ' ');
}

CollisionType Floor::CheckCollision (Drawable & other) {
    return other.CheckCollision(*this);
}

CollisionType Floor::CheckCollision (Knight & other) {
    return NO_COLLISION;
}

CollisionType Floor::CheckCollision (Princess & other) {
    return NO_COLLISION;
}

CollisionType Floor::CheckCollision (Monster & other) {
    return NO_COLLISION;
}

CollisionType Floor::CheckCollision (Wall & other) {
    return ERROR_COLLISION;
}

CollisionType Floor::CheckCollision (Floor & other) {
    return ERROR_COLLISION;
}

CollisionType Monster::CheckCollision (Drawable & other) {
    return other.CheckCollision(*this);
}

CollisionType Monster::CheckCollision (Knight & other) {
    return FIGHT_COLLISION;
}

CollisionType Monster::CheckCollision (Princess & other) {
    return NO_FIGHT_COLLISION;
}

CollisionType Monster::CheckCollision (Monster & other) {
    return NO_FIGHT_COLLISION;
}

CollisionType Monster::CheckCollision (Wall & other) {
    return OBJECT_COLLISION;
}

CollisionType Monster::CheckCollision (Floor & other) {
    return NO_COLLISION;
}

}