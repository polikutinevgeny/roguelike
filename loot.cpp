#include "loot.hpp"
#include <algorithm>
#include <ctime>

namespace rogue {

namespace {

const int HEALING_POTION_HP = 50;
const int DAMAGE_POTION_DMG = 10;
const int INVULNERABILITY_DUR = 20;

enum PotionType {
    HEAL = 0,
    DMG = 1,
    INVULNR = 2,
};

PotionType potions[3] = {HEAL, DMG, INVULNR};
TCODColor potion_colors[3] = {TCODColor::red, TCODColor::blue, TCODColor::darkerFuchsia};

}


Potion::Potion(int x, int y, int symbol, const TCODColor & color, const char * name, PlayerInterface * player)
    : x(x), y(y), symbol(symbol), color(color), name(name), player(player) {}

void Potion::Render(int mx, int my) {
    TCODConsole::root->setChar(x + mx, y + my, symbol);
    TCODConsole::root->setCharForeground(x + mx, y + my, color);
}

HealingPotion::HealingPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PlayerInterface * player)
    : Potion(x, y, symbol, color, name, player) {}

void HealingPotion::Use() {
    player->GetMaxHP() += HEALING_POTION_HP;
    player->GetHP() += HEALING_POTION_HP;
}

DamagePotion::DamagePotion(int x, int y, int symbol, const TCODColor & color, const char * name, PlayerInterface * player) 
    : Potion(x, y, symbol, color, name, player) {}

void DamagePotion::Use() {
    player->GetDamage() += DAMAGE_POTION_DMG;
}

InvulnerabilityPotion::InvulnerabilityPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PlayerInterface * player) 
    : Potion(x, y, symbol, color, name, player) {}

void InvulnerabilityPotion::Use() {
    player->Invulnerable() = true;
    player->InvulnerabilityPeriod() = INVULNERABILITY_DUR;
}

void ShufflePotions() {
    std::srand(std::time(NULL));
    std::random_shuffle(&potion_colors[0], &potion_colors[2]);
}

Potion* CreatePotion(int x, int y, PlayerInterface * player) {
    TCODRandom* rng = TCODRandom::getInstance();
    switch (PotionType(rng->getInt(0, 2))) {
        case HEAL:
            return new HealingPotion(x, y, '!', potion_colors[0], "healing_potion", player);
            break;
        case DMG:
            return new DamagePotion(x, y, '!', potion_colors[1], "damage_potion", player);
            break;
        case INVULNR:
            return new InvulnerabilityPotion(x, y, '!', potion_colors[2], "invulnr_potion", player);
            break;
        default:
            return NULL;
            break;
    }
}

}