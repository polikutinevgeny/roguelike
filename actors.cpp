#include "actors.hpp"
#include "libtcod.hpp"
#include <stdio.h>
#include <cmath>
#include <algorithm>

namespace rogue {

namespace {

const int TRACKING_TURNS = 3;
const int BASE_PLAYER_HP = 10000;
const int BASE_PLAYER_DMG = 100;
const int BASE_PLAYER_MP = 10000;
const int BASE_ZOMBIE_HP = 20;
const int BASE_ZOMBIE_DMG = 5;
const int BASE_DRAGON_HP = 40;
const int BASE_DRAGON_DMG = 10;
const int BASE_PROJECTILE_DMG = 20;
const int MAX_INVENTORY_SIZE = 10;
const int FIREBALL_MP_COST = 10;

}

Actor::Actor(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine)
    : x(x), y(y)
    , symbol(symbol)
    , color(color)
    , name(name)
    , blocks(true)
    , engine_(engine)
    , remove(false) {

}

void Actor::Render(int mx, int my) {
    TCODConsole::root->setChar(x + mx, y + my, symbol);
    TCODConsole::root->setCharForeground(x + mx, y + my, color);
}

Character::Character(int x, int y, int symbol, const TCODColor& color, const char* name,
    ActorCallbackInterface& engine) : Actor(x, y, symbol, color, name, engine) {}

Player::Player(int x, int y, int symbol, const TCODColor& color, const char* name,
    ActorCallbackInterface& engine) : Character(x, y, symbol, color, name, engine)
    , invulnerable(false)
    , invulnerability_period(0)
    , mp(BASE_PLAYER_MP)
    , max_mp(BASE_PLAYER_MP)
{
    hp = BASE_PLAYER_HP;
    max_hp = BASE_PLAYER_HP;
    damage = BASE_PLAYER_DMG;
}

void Player::Update() {
    //No AI - no update
}

ActorStatus Player::Act(int x, int y) {
    invulnerability_period = std::max(0, invulnerability_period - 1);
    if (invulnerability_period == 0) {
        invulnerable = false;
    }
    if (engine_.IsWall(x, y)) {
        return ActorStatus::NotMoved;
    }
    for (auto l = engine_.GetLoot().begin(); l != engine_.GetLoot().end();) {
        if ((*l)->x == x && (*l)->y == y && inventory.size() < MAX_INVENTORY_SIZE) {
            inventory.push_back(*l);
            l = engine_.GetLoot().erase(l);
        }
        else {
            ++l;
        }
    }
    for (auto a : engine_.GetActors()) {
        if (a->x == x && a->y == y/* && a->blocks*/) {
            a->Interact(*this);
            return ActorStatus::Moved;
        }
    }
    this->x = x;
    this->y = y;
    return ActorStatus::Moved;
}

void Player::Interact(Actor& other) {
    if (invulnerable) {
        return;
    }
    if (hp - other.damage <= 0) {
        engine_.Lose();
    }
    hp -= other.damage;
}

void Player::Shoot(int dx, int dy) {
    invulnerability_period = std::max(0, invulnerability_period - 1);
    if (invulnerability_period == 0) {
        invulnerable = false;
    }
    if (engine_.IsInBounds(x + dx, y + dy) && engine_.CanWalk(x + dx, y + dy) && mp >= FIREBALL_MP_COST) {
        engine_.GetActors().push_front(new Projectile(x + dx, y + dy, '@', TCODColor::red, "fireball", engine_, 1, dx, dy));
        mp -= FIREBALL_MP_COST;
    }
}

void Player::UseInventory(int n) {
    inventory[n]->Use();
    printf("You used %s potion\n", inventory[n]->name);
    inventory.erase(inventory.begin() + n);
}

int & Player::GetHP() {
    return hp;
}

int & Player::GetMaxHP() {
    return max_hp;
}

int & Player::GetDamage() {
    return damage;
}

bool & Player::Invulnerable() {
    return invulnerable;
}

int & Player::InvulnerabilityPeriod() {
    return invulnerability_period;
}

Zombie::Zombie(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine) : Monster(x, y, symbol, color, name, engine) {
    hp = BASE_ZOMBIE_HP;
    max_hp = BASE_ZOMBIE_HP;
    damage = BASE_ZOMBIE_DMG;
}

void Zombie::Interact(Actor& other) {
    hp = hp - other.damage > 0 ? hp - other.damage : 0;
    if (hp == 0) {
        blocks = false;
        remove = true;
    }
}

ActorStatus Zombie::Act(int x, int y) {
    int dx = x - this->x;
    int dy = y - this->y;
    int sdx = dx > 0 ? 1 : -1;
    int sdy = dy > 0 ? 1 : -1;
    double d = sqrt(dx * dx + dy * dy);
    if (d > 1) {
        dx = (int)std::round(dx / d);
        dy = (int)std::round(dy / d);
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
        engine_.GetPlayer()->Interact(*this);
    }
    for (auto a : engine_.GetActors()) {
        if (a != this && a->x == this->x && a->y == this->y &&
            a != engine_.GetPlayer() && a != engine_.GetPrincess()) 
        {
            a->Interact(*this);
        }
    }
    return ActorStatus::Moved;
}

Dragon::Dragon(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine) : Monster(x, y, symbol, color, name, engine) {
    hp = BASE_DRAGON_HP;
    max_hp = BASE_DRAGON_HP;
    damage = BASE_DRAGON_DMG;
}

void Dragon::Interact(Actor& other) {
    hp = hp - other.damage > 0 ? hp - other.damage : 0;
    if (hp == 0) {
        blocks = false;
        remove = true;
    }
}

ActorStatus Dragon::Act(int x, int y) {
    int dx = x - this->x;
    int dy = y - this->y;
    double d = sqrt(dx * dx + dy * dy);
    dx = (int)std::round(dx / d);
    dy = (int)std::round(dy / d);
    TCODRandom* rng = TCODRandom::getInstance();
    if (rng->getInt(0, 100) < 20 && engine_.IsInFOV(this->x, this->y) && (this->x == x || this->y == y) && 
        engine_.CanWalk(this->x + dx, this->y + dy)) {
        engine_.GetActors().push_front(new Projectile(this->x + dx, this->y + dy, '@', TCODColor::red, "fireball", engine_, 1, dx, dy));
        return ActorStatus::Moved;
    }
    int sdx = dx > 0 ? 1 : -1;
    int sdy = dy > 0 ? 1 : -1;
    if (d > 1) {
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
        engine_.GetPlayer()->Interact(*this);
    }
    for (auto a : engine_.GetActors()) {
        if (a != this && a->x == this->x && a->y == this->y && 
            a != engine_.GetPlayer() && a != engine_.GetPrincess()) 
        {
            a->Interact(*this);
        }
    }
    return ActorStatus::Moved;
}

Monster::Monster(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine)
    : Character(x, y, symbol, color, name, engine), move_count_(0) {}

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
        Act(engine_.GetPlayer()->x, engine_.GetPlayer()->y);
    }
}

void Monster::Interact(Monster & other) {}

Princess::Princess(int x, int y, int symbol, const TCODColor & color, const char * name,
    ActorCallbackInterface& engine)
    : Actor(x, y, symbol, color, name, engine) {}

void Princess::Update() {
    //No AI for princess
}

ActorStatus Princess::Act(int x, int y) {
    return ActorStatus::NotMoved;
}

void Princess::Interact(Actor& other) {
    engine_.Win();
}

Projectile::Projectile(int x, int y, int symbol, const TCODColor & color, const char * name, ActorCallbackInterface & engine, int speed, int dx, int dy) :
    Actor(x, y, symbol, color, name, engine)
    , speed(speed)
    , dx(dx), dy(dy) {
    blocks = false;
    damage = BASE_PROJECTILE_DMG;
}

void Projectile::Update() {
    for (int i = 0; i < speed; ++i) {
        if (engine_.IsInBounds(x + dx, y + dy)) {
            Act(x + dx, y + dy);
        }
        else {
            remove = true;
        }
    }
}

ActorStatus Projectile::Act(int x, int y) {
    if (engine_.IsWall(x, y)) {
        remove = true;
        engine_.Destroy(x, y);
        return ActorStatus::NotMoved;
    }
    for (auto a : engine_.GetActors()) {
        if (a->x == x && a->y == y && a->blocks && a != engine_.GetPrincess()) {
            a->Interact(*this);
            remove = true;
            return ActorStatus::Moved;
        }
    }
    this->x = x;
    this->y = y;
    return ActorStatus::Moved;
}

void Projectile::Interact(Actor & other) {
    other.Interact(*this);
    remove = true;
}

}