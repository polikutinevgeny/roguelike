#pragma once
#include "libtcod.hpp"
#include "drawable.hpp"

namespace rogue {

class PotionPlayerCallbackInterface {
public:
    virtual int& GetHP() = 0;
    virtual int& GetMaxHP() = 0;
    virtual int& GetMP() = 0;
    virtual int& GetMaxMP() = 0;
    virtual int& GetDamage() = 0;
    virtual bool& Invulnerable() = 0;
    virtual int& InvulnerabilityPeriod() = 0;
};

class PotionMapCallbackInterface {
public:
    virtual void OpenMap() = 0;
};

class Potion : public Drawable {
public:
    Potion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    virtual void Use() = 0;

    PotionPlayerCallbackInterface* player;
    PotionMapCallbackInterface* map;
};

class MaxHealthPotion : public Potion {
public:
    MaxHealthPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class DamagePotion : public Potion {
public:
    DamagePotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class InvulnerabilityPotion : public Potion {
public:
    InvulnerabilityPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class HealingPotion : public Potion {
public:
    HealingPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class MaxManaPotion : public Potion {
public:
    MaxManaPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class ManaPotion : public Potion {
public:
    ManaPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

class RevealMapPotion : public Potion {
public:
    RevealMapPotion(int x, int y, int symbol, const TCODColor& color, const char* name, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

    void Use() override;
};

void ShufflePotions();
Potion* CreatePotion(int x, int y, PotionPlayerCallbackInterface* player, PotionMapCallbackInterface* map);

}