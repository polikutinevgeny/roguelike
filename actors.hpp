#pragma once
#include "libtcod.hpp"
#include <list>

namespace rogue {

enum class ActorStatus {
    Moved,
    NotMoved,
};

class Actor;
class Player;

class ActorCallbackInterface {
public:
    virtual void Win() = 0;
    virtual void Lose() = 0;
    virtual bool IsWall(int x, int y) = 0;
    virtual bool CanWalk(int x, int y) = 0;
    virtual bool IsInFOV(int x, int y) = 0;
    virtual bool IsInBounds(int x, int y) = 0;
    virtual void Destroy(int x, int y) = 0;
    virtual Player* GetPlayer() = 0;
    virtual Actor* GetPrincess() = 0;
    virtual std::list<Actor*>& GetActors() = 0;
};

class Character;

class Actor {
public:
    Actor(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Render();

    virtual void Update() = 0;

    virtual ActorStatus Act(int x, int y) = 0;

    virtual void Interact(Actor& other) = 0;

    int x, y;
    int symbol;
    TCODColor color;
    const char* name;

    bool blocks;

    bool remove;

    int damage;

protected:
    ActorCallbackInterface& engine_;
};

class Character : public Actor {
public:
    Character(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);
    int hp;
    int max_hp;
};

class Player : public Character {
public:
    Player(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Update() override;

    ActorStatus Act(int x, int y) override;

    void Interact(Actor& other) override;

    void Shoot(int dx, int dy);
};

class Monster : public Character {
public:
    Monster(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Update() override;

    void Interact(Monster& other);

protected:
    int move_count_;
};

class Zombie : public Monster {
public:
    Zombie(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    ActorStatus Act(int x, int y) override;

    void Interact(Actor& other) override;
};

class Dragon : public Monster {
public:
    Dragon(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    ActorStatus Act(int x, int y) override;

    void Interact(Actor& other) override;
};

class Princess : public Actor {
public:
    Princess(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine);

    void Update() override;

    ActorStatus Act(int x, int y) override;

    void Interact(Actor& other) override;
};

class Projectile : public Actor {
public:
    Projectile(int x, int y, int symbol, const TCODColor& color, const char* name,
        ActorCallbackInterface& engine, int speed, int dx, int dy);

    void Update() override;

    ActorStatus Act(int x, int y) override;

    void Interact(Actor& other) override;

    int speed;
    int dx, dy;

};

}