#pragma once
#include "libtcod.hpp"

namespace rogue {

class PlayerInterface {
public:
    virtual int& GetHP() = 0;
    virtual int& GetMaxHP() = 0;
    virtual int& GetDamage() = 0;
    virtual bool& Invulnerable() = 0;
    virtual int& InvulnerabilityPeriod() = 0;
};

class Potion {
public:
    Potion(int x, int y, int symbol, const TCODColor& color, const char* name, PlayerInterface* player);

    virtual void Use() = 0;

    void Render(int mx, int my);

    int x, y;
    int symbol;
    TCODColor color;
    const char* name;

    PlayerInterface* player;
};

class HealingPotion : public Potion {
public:
    HealingPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PlayerInterface* player);

    void Use() override;
};

class DamagePotion : public Potion {
public:
    DamagePotion(int x, int y, int symbol, const TCODColor& color, const char* name, PlayerInterface* player);

    void Use() override;
};

class InvulnerabilityPotion : public Potion {
public:
    InvulnerabilityPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PlayerInterface* player);

    void Use() override;
};

void ShufflePotions();
Potion* CreatePotion(int x, int y, PlayerInterface* player);

}