#include "game_map.hpp"

namespace rogue {

Game::Game (int height, int width) : width_ (width), height_ (height), objects_ (height, std::vector<Drawable*> (width)) {
    for (int i = 0; i < objects_.size (); ++i) {
        for (int j = 0; j < objects_[i].size(); ++j) {
            objects_[i][j] = new Floor(i, j);
        }
    }
    player_ = NULL;
    princess_ = NULL;
}

Game::~Game () {
    for (std::vector<Drawable*> i : objects_) {
        for (Drawable* j : i) {
            delete j;
        }
    }
    delete player_;
    delete princess_;
    for (std::vector<Monster*> i : enemies_) {
        for (Monster* j : i) {
            delete j;
        }
    }
}

void Game::PutPlayer (int x, int y) {
    player_ = new Knight (x, y, 40);
}

void Game::MovePlayer (int dx, int dy) {
    player_->Move (dx, dy);
}

void Game::PutPrincess (int x, int y) {
    princess_ = new Princess (x, y);
}

void rogue::Game::Render () {
    for (std::vector<Drawable*> i : objects_) {
        for (Drawable* j : i) {
            j->Draw ();
        }
    }
    player_->Draw ();
    princess_->Draw ();
}

}