#include "libtcod.hpp"
#include "drawable.hpp"
#include "game_map.hpp"
#include "engine.hpp"

rogue::Engine::Engine() : fov_radius(10), compute_fov_(true), status_(STARTUP) {
    TCODConsole::initRoot(80, 50, "Rogue-like game", false);
    player = new Actor(40, 25, 'K', TCODColor::white);
    actors.push_back(player);
    map = new Map(80, 45);
}

rogue::Engine::~Engine() {
    for (auto a : actors) {
        delete a;
    }
    delete map;
}

void rogue::Engine::Update() {
    TCOD_key_t key;
    if (status_ == STARTUP) {
        map->ComputeFOV();
    }
    status_ = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    int dx = 0;
    int dy = 0;
    switch (key.vk) {
        case TCODK_UP: 
            dy = -1;
            break;
        case TCODK_DOWN: 
            dy = 1;
            break;
        case TCODK_LEFT: 
            dx = -1;
            break;
        case TCODK_RIGHT: 
            dx = 1;
            break;
        default:break;
    }
    if (dx || dy) {
        status_ = NEW_TURN;
        if (player->MoveOrAttack(player->x + dx, player->y + dy)) {
            map->ComputeFOV();
        }
    }
    if (status_ = NEW_TURN) {
        for (auto a : actors) {
            if (a != player) {
                a->Update();
            }
        }
    }
}

void rogue::Engine::Render() {
    TCODConsole::root->clear();
    map->Render();
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y)) {
            a->Render();
        }
    }
}