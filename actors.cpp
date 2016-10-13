#include "actors.hpp"
#include "engine.hpp"
#include "libtcod.hpp"
#include <stdio.h>

namespace rogue {

static const int TRACKING_TURNS = 3;
static const int BASE_PLAYER_HP = 10000;
static const int BASE_PLAYER_DMG = 100;
static const int BASE_ZOMBIE_HP = 20;
static const int BASE_ZOMBIE_DMG = 5;
static const int BASE_DRAGON_HP = 40;
static const int BASE_DRAGON_DMG = 10;

Actor::Actor(int x, int y, int face, const TCODColor & color, const char * name) :
    x(x), y(y), face(face), color(color), name(name), hp(-1), max_hp(-1), damage(0), blocks(true) {
}

void Actor::Render() {
    TCODConsole::root->setChar(x, y, face);
    TCODConsole::root->setCharForeground(x, y, color);
}

Player::Player(int x, int y, int face, const TCODColor& color, const char* name) :
    Actor(x, y, face, color, name) {
    hp = BASE_PLAYER_HP;
    max_hp = BASE_PLAYER_HP;
    damage = BASE_PLAYER_DMG;
}

void Player::Update() {
    //No AI - no update
}

bool Player::MoveOrAttack(int x, int y) {
    if (engine.map->IsWall(x, y)) {
        return false;
    }
    for (auto a : engine.actors) {
        if (a->x == x && a->y == y && a->blocks) {
            a->GetDamage(this->damage);
            return true;
        }
    }
    this->x = x;
    this->y = y;
    return true;
}

void Player::GetDamage(int dmg) {
    if (hp - dmg <= 0) {
        engine.Lose();
    }
    hp -= dmg;
}

Zombie::Zombie(int x, int y, int face, const TCODColor & color, const char * name) :
    Monster(x, y, face, color, name) {
    hp = BASE_ZOMBIE_HP;
    max_hp = BASE_ZOMBIE_HP;
    damage = BASE_ZOMBIE_DMG;
}

void Zombie::GetDamage(int dmg) {
    hp = hp - dmg > 0 ? hp - dmg : 0;
    if (hp == 0) {
        blocks = false;
        face = '+';
    }
}

Dragon::Dragon(int x, int y, int face, const TCODColor & color, const char * name) :
    Monster(x, y, face, color, name) {
    hp = BASE_DRAGON_HP;
    max_hp = BASE_DRAGON_HP;
    damage = BASE_DRAGON_DMG;
}

void Dragon::GetDamage(int dmg) {
    hp = hp - dmg > 0 ? hp - dmg : 0;
    if (hp == 0) {
        blocks = false;
        face = '+';
    }
}

Monster::Monster(int x, int y, int face, const TCODColor & color, const char * name) :
    Actor(x, y, face, color, name) {}

Monster::~Monster() {}

void Monster::Update() {
    if (hp == 0) {
        return;
    }
    if (engine.map->IsInFOV(x, y)) {
        move_count_ = TRACKING_TURNS;
    }
    else {
        move_count_--;
    }
    if (move_count_ > 0) {
        MoveOrAttack(engine.player->x, engine.player->y);
    }
}

bool Monster::MoveOrAttack(int x, int y) {
    int dx = x - this->x;
    int dy = y - this->y;
    int sdx = dx > 0 ? 1 : -1;
    int sdy = dy > 0 ? 1 : -1;
    double d = sqrt(dx * dx + dy * dy);
    if (d > 1) {
        dx = (int) std::round(dx / d);
        dy = (int) std::round(dy / d);
        if (engine.map->CanWalk(this->x + dx, this->y + dy)) {
            this->x += dx;
            this->y += dy;
        }
        else if (engine.map->CanWalk(this->x + sdx, this->y)) {
            this->x += sdx;
        }
        else if (engine.map->CanWalk(this->x, this->y + sdy)) {
            this->y += sdy;
        }
    }
    else {
        engine.player->GetDamage(this->damage);
    }
    return true;
}

Princess::Princess(int x, int y, int face, const TCODColor & color, const char * name) : 
    Actor(x, y, face, color, name) {}

void Princess::Update() {
    //No AI for princess
}

bool Princess::MoveOrAttack(int x, int y) {
    return false;
}

void Princess::GetDamage(int dmg) {
    engine.Win();
}

}