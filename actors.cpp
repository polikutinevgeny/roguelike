#include "actors.hpp"
#include "libtcod.hpp"
#include <stdio.h>
#include <cmath>

namespace rogue {

namespace {

const int TRACKING_TURNS = 3;
const int BASE_PLAYER_HP = 10000;
const int BASE_PLAYER_DMG = 100;
const int BASE_ZOMBIE_HP = 20;
const int BASE_ZOMBIE_DMG = 5;
const int BASE_DRAGON_HP = 40;
const int BASE_DRAGON_DMG = 10;

}

Actor::Actor(int x, int y, int symbol, const TCODColor & color, const char * name,
            ActorCallbackInterface& engine) 
    : x(x), y(y), symbol(symbol)
    , color(color)
    , name(name), hp(-1)
    , max_hp(-1), damage(0), blocks(true), engine_(engine) 
{
            
}

void Actor::Render() {
    TCODConsole::root->setChar(x, y, symbol);
    TCODConsole::root->setCharForeground(x, y, color);
}

Player::Player(int x, int y, int symbol, const TCODColor& color, const char* name,
    ActorCallbackInterface& engine) : Actor(x, y, symbol, color, name, engine) {
    hp = BASE_PLAYER_HP;
    max_hp = BASE_PLAYER_HP;
    damage = BASE_PLAYER_DMG;
}

void Player::Update() {
    //No AI - no update
}

ActorStatus Player::MoveOrAttack(int x, int y) {
    if (engine_.IsWall(x, y)) {
        return ActorStatus::NotMoved;
    }
    for (auto a : engine_.GetActors()) {
        if (a->x == x && a->y == y && a->blocks) {
            a->RecieveDamage(this->damage);
            return ActorStatus::Moved;
        }
    }
    this->x = x;
    this->y = y;
    return ActorStatus::Moved;
}

void Player::RecieveDamage(int dmg) {
    if (hp - dmg <= 0) {
        engine_.Lose();
    }
    hp -= dmg;
}

Zombie::Zombie(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine) : Monster(x, y, symbol, color, name, engine) {
    hp = BASE_ZOMBIE_HP;
    max_hp = BASE_ZOMBIE_HP;
    damage = BASE_ZOMBIE_DMG;
}

void Zombie::RecieveDamage(int dmg) {
    hp = hp - dmg > 0 ? hp - dmg : 0;
    if (hp == 0) {
        blocks = false;
        symbol = '+';
    }
}

Dragon::Dragon(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine) : Monster(x, y, symbol, color, name, engine) {
    hp = BASE_DRAGON_HP;
    max_hp = BASE_DRAGON_HP;
    damage = BASE_DRAGON_DMG;
}

void Dragon::RecieveDamage(int dmg) {
    hp = hp - dmg > 0 ? hp - dmg : 0;
    if (hp == 0) {
        blocks = false;
        symbol = '+';
    }
}

Monster::Monster(int x, int y, int symbol, const TCODColor & color, const char * name,
        ActorCallbackInterface& engine) 
    : Actor(x, y, symbol, color, name, engine), move_count_(0) {}

Monster::~Monster() {}

void Monster::Update() {
    if (hp == 0) {
        return;
    }
    if (engine_.IsInFOV(x, y)) {
        move_count_ = TRACKING_TURNS;
    }
    else {
        move_count_--;
    }
    if (move_count_ > 0) {
        MoveOrAttack(engine_.GetPlayer().x, engine_.GetPlayer().y);
    }
}

ActorStatus Monster::MoveOrAttack(int x, int y) {
    int dx = x - this->x;
    int dy = y - this->y;
    int sdx = dx > 0 ? 1 : -1;
    int sdy = dy > 0 ? 1 : -1;
    double d = sqrt(dx * dx + dy * dy);
    if (d > 1) {
        dx = (int) std::round(dx / d);
        dy = (int) std::round(dy / d);
        if (engine_.CanWalk(this->x + dx, this->y + dy) && ((dx != 0) != (dy != 0))) {
            this->x += dx;
            this->y += dy;
        }
        else if (engine_.CanWalk(this->x + sdx, this->y)) {
            this->x += sdx;
        }
        else if (engine_.CanWalk(this->x, this->y + sdy)) {
            this->y += sdy;
        }
        else {
            return ActorStatus::NotMoved;
        }
    }
    else {
        engine_.GetPlayer().RecieveDamage(this->damage);
    }
    return ActorStatus::Moved;
}

Princess::Princess(int x, int y, int symbol, const TCODColor & color, const char * name,
        ActorCallbackInterface& engine) 
    : Actor(x, y, symbol, color, name, engine) {}

void Princess::Update() {
    //No AI for princess
}

ActorStatus Princess::MoveOrAttack(int x, int y) {
    return ActorStatus::NotMoved;
}

void Princess::RecieveDamage(int dmg) {
    engine_.Win();
}

}