#include "libtcod.hpp"
#include "engine.hpp"
#include "config.hpp"
#include "color.hpp"

namespace rogue {

namespace {

const int GUI_PANEL_HEIGHT = GetConfigValue("GUI_PANEL_HEIGHT");
const int GUI_PANEL_WIDTH = GetConfigValue("GUI_PANEL_WIDTH");

const Color PLAYER_COLOR(GetConfigValue("PLAYER_COLOR"));
const Color PRINCESS_COLOR(GetConfigValue("PRINCESS_COLOR"));

const int PLAYER_SYM = GetConfigValue("PLAYER_SYM");
const int PRINCESS_SYM = GetConfigValue("PRINCESS_SYM");

}

Engine::Engine(int width, int height) 
    : fov_radius(10), compute_fov_(true), status_(GameStatus::STARTUP), width(width), height(height)
{
    ShufflePotions();
    TCODConsole::setCustomFont("terminal.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE, 32, 8);
    TCODConsole::initRoot(width, height, "Rogue-like game", false);
    player = new Player(40, 25, PLAYER_SYM, PLAYER_COLOR, "player", *this);
    princess = new Princess(40, 30, PRINCESS_SYM, PRINCESS_COLOR, "princess", *this);
    actors.push_back(player);
    actors.push_back(princess);
    map = new Map(width, height - GUI_PANEL_HEIGHT, *this, *this);
    gui_ = new Gui(GUI_PANEL_WIDTH, GUI_PANEL_HEIGHT, width, height, *this);
}

Engine::~Engine() {
    for (auto a : actors) {
        delete a;
    }
    for (auto l : loot) {
        delete l;
    }
    delete map;
    delete gui_;
}

void Engine::Update() {
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
        case TCODK_0:
        case TCODK_1:
        case TCODK_2:
        case TCODK_3:
        case TCODK_4:
        case TCODK_5:
        case TCODK_6:
        case TCODK_7:
        case TCODK_8:
        case TCODK_9:
            message = player->UseInventory(stoi(std::string(1, key.c)));
            break;
        default:
            switch (key.c) {
                case 'w':
                    player->Shoot(0, -1);
                    status_ = GameStatus::NEW_TURN;
                    break;
                case 'a':
                    player->Shoot(-1, 0);
                    status_ = GameStatus::NEW_TURN;
                    break;
                case 's':
                    player->Shoot(0, 1);
                    status_ = GameStatus::NEW_TURN;
                    break;
                case 'd':
                    player->Shoot(1, 0);
                    status_ = GameStatus::NEW_TURN;
                    break;
                default:
                    break;
            }
            break;
    }
    if (dx || dy) {
        if (player->Act(player->x + dx, player->y + dy) == ActorStatus::Moved && 
            status_ != GameStatus::VICTORY)
        {
            status_ = GameStatus::NEW_TURN;
            map->ComputeFOV();
        }
    }
    if (status_ == GameStatus::NEW_TURN) {
        for (auto a = actors.begin(); a != actors.end();) {
            if (*a != player) {
                (*a)->Update();
                if ((*a)->remove) {
                    delete *a;
                    a = actors.erase(a);
                }
                else {
                    ++a;
                }
                continue;
            }
            ++a;
        }
        actors.remove_if([](Actor* a) {if (a->remove) { delete a; return true; } return false; });
        printf("Princess dx: %d, dy %d\n", princess->x - player->x, princess->y - player->y);
    }
    
}

void Engine::Render() {
    TCODConsole::root->clear();
    if (status_ == GameStatus::VICTORY) {
        TCODConsole::root->printEx(width / 2, height / 2, TCOD_BKGND_NONE, TCOD_CENTER, "YOU WON");
        return;
    }
    if (status_ == GameStatus::DEFEAT) {
        TCODConsole::root->printEx(width / 2, height / 2, TCOD_BKGND_NONE, TCOD_CENTER, "YOU LOST");
        return;
    }
    map->Render(0, GUI_PANEL_HEIGHT);
    for (auto l : loot) {
        if (map->IsInFOV(l->x, l->y)) {
            l->Render(0, GUI_PANEL_HEIGHT);
        }
    }
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && !a->blocks) {
            a->Render(0, GUI_PANEL_HEIGHT);
        }
    }
    for (auto a : actors) {
        if (map->IsInFOV(a->x, a->y) && a->blocks) {
            a->Render(0, GUI_PANEL_HEIGHT);
        }
    }
    gui_->Render(message);
    message = "";
}

void Engine::Lose() {
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

bool Engine::IsInBounds(int x, int y) {
    return map->IsInBounds(x, y);
}

void Engine::Destroy(int x, int y) {
    map->Destroy(x, y);
}

Player* Engine::GetPlayer() {
    return player;
}

Actor* Engine::GetPrincess() {
    return princess;
}

std::list<Actor*>& Engine::GetActors() {
    return actors;
}

int Engine::GetFOVRadius() {
    return fov_radius;
}

std::list<Potion*>& Engine::GetLoot() {
    return loot;
}

void Engine::OpenMap() {
    map->OpenMap();
}

void rogue::Engine::Win() {
    status_ = GameStatus::VICTORY;
}

}