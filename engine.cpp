#include "libtcod.hpp"
#include "actors.hpp"
#include "game_map.hpp"
#include "engine.hpp"

namespace rogue {

rogue::Engine::Engine(int width, int height) :
    fov_radius(10), compute_fov_(true), status_(STARTUP), width(width), height(height) {
    TCODConsole::initRoot(width, height, "Rogue-like game", false);
    player = new Player(40, 25, 'K', TCODColor::white, "player");
    princess = new Princess(40, 30, 'P', TCODColor::darkerGrey, "princess");
    actors.push_back(player);
    actors.push_back(princess);
    map = new Map(width, height - GUI_PANEL_HEIGHT);
    gui_ = new Gui();
}

rogue::Engine::~Engine() {
    for (auto a : actors) {
        delete a;
    }
    delete map;
    delete gui_;
}

void rogue::Engine::Update() {
    TCOD_key_t key;
    if (status_ == STARTUP) {
        map->ComputeFOV();
    }
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    if (status_ == VICTORY || status_ == DEFEAT) {
        if (key.vk == TCODK_ENTER) {
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
        if (player->MoveOrAttack(player->x + dx, player->y + dy) && status_ != VICTORY) {
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
        printf("Princess dx: %d, dy %d\n", princess->x - player->x, princess->y - player->y);
    }
}

void rogue::Engine::Render() {
    TCODConsole::root->clear();
    if (status_ == VICTORY) {
        TCODConsole::root->printEx(width / 2, height / 2, TCOD_BKGND_NONE, TCOD_CENTER, "YOU WON");
        return;
    }
    if (status_ == DEFEAT) {
        TCODConsole::root->printEx(width / 2, height / 2, TCOD_BKGND_NONE, TCOD_CENTER, "YOU LOST");
        return;
    }
    map->Render();
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && !a->blocks) {
            a->Render();
        }
    }
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && a->blocks) {
            a->Render();
        }
    }
    gui_->Render();
}

void rogue::Engine::Lose() {
    status_ = DEFEAT;
}

void rogue::Engine::Win() {
    status_ = VICTORY;
}

}