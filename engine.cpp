#include "libtcod.hpp"
#include "drawable.hpp"
#include "game_map.hpp"
#include "engine.hpp"

rogue::Engine::Engine(int width, int height) : 
    fov_radius(10), compute_fov_(true), status_(STARTUP), width(width), height(height) {
    TCODConsole::initRoot(width, height, "Rogue-like game", false);
    player = new Player(40, 25, 'K', TCODColor::white, "player");
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
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    if (status_ == VICTORY || status_ == DEFEAT) {
        if (key.vk != TCODK_NONE) {
            exit(0);
        }
        return;
    }
    int dx = 0;
    int dy = 0;
    status_ = IDLE;
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
        if (player->MoveOrAttack(player->x + dx, player->y + dy)) {
            status_ = NEW_TURN;
            map->ComputeFOV();
        }
    }
    if (status_ == NEW_TURN) {
        for (auto a : actors) {
            if (a != player) {
                a->Update();
            }
        }
    }
}

void rogue::Engine::Render() {
    TCODConsole::root->clear();
    if (status_ == VICTORY) {
        TCODConsole::root->print(1, height - 2, "YOU WON");
        return;
    }
    if (status_ == DEFEAT) {
        TCODConsole::root->print(1, height - 2, "YOU LOST");
        return;
    }
    map->Render();
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && a != player && a->blocks) {
            a->Render();
        }
    }
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && !a->blocks) {
            a->Render();
        }
    }
    player->Render();
    TCODConsole::root->print(1, height - 2, "HP : %d", player->hp);
}

void rogue::Engine::Lose() {
    status_ = DEFEAT;
}

void rogue::Engine::Win() {
    status_ = VICTORY;
}
