#pragma once
#include "drawable.hpp"
#include <vector>

namespace rogue {

class Game {

public:
    Game (int height, int width);
    ~Game ();

    void PutPlayer (int x, int y);
    void MovePlayer (int dx, int dy);

    void PutPrincess (int x, int y);

    void Render ();

protected:
    int width_;
    int height_;

    Knight* player_;
    Princess* princess_;

    std::vector<std::vector<Monster*>> enemies_;

    std::vector<std::vector<Drawable*>> objects_;

};

}