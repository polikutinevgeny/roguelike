#pragma once
#include "libtcod.hpp"
#include <list>

namespace rogue {

class Actor;

class ActorCallbackInterface {
public:
    virtual void Win() = 0;
    virtual void Lose() = 0;
    virtual bool IsWall(int x, int y) = 0;
    virtual bool CanWalk(int x, int y) = 0;
    virtual bool IsInFOV(int x, int y) = 0;
    virtual Actor& GetPlayer() = 0;
    virtual std::list<Actor*>& GetActors() = 0;
};

class Actor {
public:
    Actor(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Render();

    virtual void Update() = 0;

    virtual bool MoveOrAttack(int x, int y) = 0;

    virtual void RecieveDamage(int dmg) = 0;

    int x, y;
    int face;
    TCODColor color;
    const char* name;

    int hp;
    int max_hp;
    int damage;

    bool blocks;

protected:
    ActorCallbackInterface& engine_;
};

class Player : public Actor {
public:
    Player(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

    void RecieveDamage(int dmg) override;
};

class Monster : public Actor {
public:
    Monster(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);
    virtual ~Monster() = 0; //we need class to be abstract

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

protected:
    int move_count_;
};

class Zombie : public Monster {
public:
    Zombie(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void RecieveDamage(int dmg) override;
};

class Dragon : public Monster {
public:
    Dragon(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void RecieveDamage(int dmg) override;
};

class Princess : public Actor {
public:
    Princess(int x, int y, int face, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Update() override;

    bool MoveOrAttack(int x, int y) override;

    void RecieveDamage(int dmg) override;
};

}