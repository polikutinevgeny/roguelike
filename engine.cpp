#include "libtcod.hpp"
#include "engine.hpp"

namespace rogue {

namespace {

const int GUI_PANEL_HEIGHT = 7;
const int GUI_PANEL_WIDTH = 40;

}

rogue::Engine::Engine(int width, int height) 
    : fov_radius(10), compute_fov_(true), status_(GameStatus::STARTUP), width(width), height(height)
{
    TCODConsole::setCustomFont("terminal.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE, 32, 8);
    TCODConsole::initRoot(width, height, "Rogue-like game", false);
    player = new Player(40, 25, 'K', TCODColor::white, "player", *this);
    princess = new Princess(40, 30, 'P', TCODColor::darkerGrey, "princess", *this);
    actors.push_back(player);
    actors.push_back(princess);
    map = new Map(width, height - GUI_PANEL_HEIGHT, *this, *this);
    gui_ = new Gui(GUI_PANEL_WIDTH, GUI_PANEL_HEIGHT, width, height, *this);
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
    if (status_ == GameStatus::STARTUP) {
        map->ComputeFOV();
        Render();
        TCODConsole::root->flush();
    }
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, false);
    if (status_ == GameStatus::VICTORY || status_ == GameStatus::DEFEAT) {
        if (key.vk == TCODK_ENTER) {
            exit(0);
        }
        return;
    }
    int dx = 0;
    int dy = 0;
    status_ = GameStatus::IDLE;
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
        if (player->MoveOrAttack(player->x + dx, player->y + dy) == ActorStatus::Moved && 
            status_ != GameStatus::VICTORY)
        {
            status_ = GameStatus::NEW_TURN;
            map->ComputeFOV();
        }
    }
    if (status_ == GameStatus::NEW_TURN) {
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
    if (status_ == GameStatus::VICTORY) {
        TCODConsole::root->printEx(width / 2, height / 2, TCOD_BKGND_NONE, TCOD_CENTER, "YOU WON");
        return;
    }
    if (status_ == GameStatus::DEFEAT) {
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
    status_ = GameStatus::DEFEAT;
}

bool Engine::IsWall(int x, int y) {
    return map->IsWall(x, y);
}

bool Engine::CanWalk(int x, int y) {
    return map->CanWalk(x, y);
}

bool Engine::IsInFOV(int x, int y) {
    return map->IsInFOV(x, y);
}

Actor& Engine::GetPlayer() {
    return *player;
}

Actor& Engine::GetPrincess() {
    return *princess;
}

std::list<Actor*>& Engine::GetActors() {
    return actors;
}

int Engine::GetFOVRadius() {
    return fov_radius;
}

void rogue::Engine::Win() {
    status_ = GameStatus::VICTORY;
}

}