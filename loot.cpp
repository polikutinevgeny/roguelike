#include "loot.hpp"
#include <algorithm>
#include <ctime>

namespace rogue {

namespace {

const int MAX_HEALTH_POTION_HP = 50;
const int HEALING_POTION_HP = 100;
const int MAX_MANA_POTION_MP = 50;
const int MANA_POTION_MP = 100;
const int DAMAGE_POTION_DMG = 10;
const int INVULNERABILITY_DUR = 20;

enum PotionType {
    MAX_HP = 0,
    DMG = 1,
    INVULNR = 2,
    HEALING = 3,
    MAX_MP = 4,
    MANA = 5,
    MAP = 6,
};

PotionType potions[] = {MAX_HP, DMG, INVULNR, HEALING, MAX_MP, MANA, MAP};
TCODColor potion_colors[] = {TCODColor::red, TCODColor::blue, TCODColor::darkerFuchsia, TCODColor::azure, TCODColor::cyan, TCODColor::desaturatedSea, TCODColor::darkestSea};

const int pot_num = sizeof(potions) / sizeof(PotionType);

}


Potion::Potion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map)
    : Drawable(x, y, symbol, color, name)
    , player(player)
    , map(map) {}


MaxHealthPotion::MaxHealthPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map)
    : Potion(x, y, symbol, color, name, player, map) {}

void MaxHealthPotion::Use() {
    player->GetMaxHP() += MAX_HEALTH_POTION_HP;
    player->GetHP() += MAX_HEALTH_POTION_HP;
}

DamagePotion::DamagePotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map)
    : Potion(x, y, symbol, color, name, player, map) {}

void DamagePotion::Use() {
    player->GetDamage() += DAMAGE_POTION_DMG;
}

InvulnerabilityPotion::InvulnerabilityPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map)
    : Potion(x, y, symbol, color, name, player, map) {}

void InvulnerabilityPotion::Use() {
    player->Invulnerable() = true;
    player->InvulnerabilityPeriod() = INVULNERABILITY_DUR;
}

void ShufflePotions() {
    std::srand(std::time(NULL));
    std::random_shuffle(&potion_colors[0], &potion_colors[pot_num - 1]);
}

Potion* CreatePotion(int x, int y, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map) {
    TCODRandom* rng = TCODRandom::getInstance();
    switch (PotionType(rng->getInt(0, pot_num - 1))) {
        case MAX_HP:
            return new MaxHealthPotion(x, y, '!', potion_colors[0], "Max health increased", player, map);
            break;
        case DMG:
            return new DamagePotion(x, y, '!', potion_colors[1], "Damage increased", player, map);
            break;
        case INVULNR:
            return new InvulnerabilityPotion(x, y, '!', potion_colors[2], "Invulnerability granted", player, map);
            break;
        case HEALING:
            return new HealingPotion(x, y, '!', potion_colors[3], "Healed", player, map);
            break;
        case MAX_MP:
            return new MaxManaPotion(x, y, '!', potion_colors[4], "Max mana increased", player, map);
            break;
        case MANA:
            return new ManaPotion(x, y, '!', potion_colors[5], "Mana refilled", player, map);
            break;
        case MAP:
            return new RevealMapPotion(x, y, '!', potion_colors[6], "Knowledge flows into your head", player, map);
            break;
        default:
            return NULL;
            break;
    }
}

HealingPotion::HealingPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map) : Potion(x, y, symbol, color, name, player, map) {}

void HealingPotion::Use() {
    player->GetHP() += HEALING_POTION_HP;
    player->GetHP() = std::min(player->GetHP(), player->GetMaxHP());
}

MaxManaPotion::MaxManaPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map) : Potion(x, y, symbol, color, name, player, map) {}

void MaxManaPotion::Use() {
    player->GetMaxMP() += MAX_MANA_POTION_MP;
    player->GetMP() += MAX_MANA_POTION_MP;
}

ManaPotion::ManaPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map) : Potion(x, y, symbol, color, name, player, map) {}

void ManaPotion::Use() {
    player->GetMP() += MANA_POTION_MP;
    player->GetMP() = std::min(player->GetMP(), player->GetMaxMP());
}

RevealMapPotion::RevealMapPotion(int x, int y, int symbol, const TCODColor & color, const char * name, PotionPlayerCallbackInterface * player, PotionMapCallbackInterface* map) : Potion(x, y, symbol, color, name, player, map) {}

void RevealMapPotion::Use() {
    map->OpenMap();
}

}