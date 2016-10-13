#pragma once
#include <list>
#include <string>
#include "game_map.hpp"
#include "gui.hpp"

namespace rogue {

enum GameStatus {
    STARTUP,
    IDLE,
    NEW_TURN,
    VICTORY,
    DEFEAT,
};

class Engine {
public:
    Engine(int width, int height);
    ~Engine();

    void Update();
    void Render();

    void Lose();
    void Win();

    std::list<Actor*> actors;
    Player* player;
    Princess* princess;
    Map* map;

    int fov_radius;
    int width;
    int height;

    Gui* gui;

private:
    bool compute_fov_;

    GameStatus status_;

};

extern Engine engine;

}