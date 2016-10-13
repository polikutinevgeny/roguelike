#pragma once
#include "libtcod.hpp"

namespace rogue {

class Actor {
public:
    Actor(int x, int y, int face, const TCODColor& color, const char* name);

    void Render();

    virtual void Update() = 0;

    virtual bool MoveOrAttack(int x, int y) = 0;

    virtual void GetDamage(int dmg) = 0;

    int x, y;
    int face;
    TCODColor color;
    const char* name;

    int hp;
    int max_hp;
    int damage;

    bool blocks;
};

class Player : public Actor {
public:
    Player(int x, int y, int face, const TCODColor& color, const char* name);

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

    void GetDamage(int dmg) override;
};

class Monster : public Actor {
public:
    Monster(int x, int y, int face, const TCODColor& color, const char* name);
    virtual ~Monster() = 0; //we need class to be abstract

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

protected:
    int move_count_;
};

class Zombie : public Monster {
public:
    Zombie(int x, int y, int face, const TCODColor& color, const char* name);

    void GetDamage(int dmg) override;
};

class Dragon : public Monster {
public:
    Dragon(int x, int y, int face, const TCODColor& color, const char* name);

    void GetDamage(int dmg) override;
};

class Princess : public Actor {
public:
    Princess(int x, int y, int face, const TCODColor& color, const char* name);

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

    void GetDamage(int dmg) override;
};

}