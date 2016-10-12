#pragma once
#include <list>
#include <string>
#include "game_map.hpp"

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
    Engine();
    ~Engine();

    void Update();
    void Render();

    std::list<Actor*> actors;
    Actor* player;
    Map* map;

    int fov_radius;

private:
    bool compute_fov_;

    GameStatus status_;

};

extern Engine engine;

}