#pragma once
#include <list>
#include <string>
#include "game_map.hpp"
#include "gui.hpp"
#include "actors.hpp"

namespace rogue {

class Engine : public ActorCallbackInterface, public GuiCallbackInterface, public MapCallbackInterface {
public:
    Engine(int width, int height);
    ~Engine();

    void Update();
    void Render();

    void Win() override;
    void Lose() override;
    bool IsWall(int x, int y) override;
    bool CanWalk(int x, int y) override;
    bool IsInFOV(int x, int y) override;
    bool IsInBounds(int x, int y) override;
    void Destroy(int x, int y) override;
    Player* GetPlayer() override;
    Actor* GetPrincess() override;
    std::list<Actor*>& GetActors() override;
    int GetFOVRadius() override;

    std::list<Actor*> actors;
    Player* player;
    Princess* princess;
    Map* map;

    int fov_radius;
    int width;
    int height;

private:
    enum class GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT,
    };

    bool compute_fov_;

    Gui* gui_;

    GameStatus status_;

};

extern Engine engine;

}