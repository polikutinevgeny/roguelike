#include "game_map.hpp"
#include <random>
#include <ctime>
#include <queue>

namespace rogue {

int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };

Game::Game (int height, int width) : width_ (width), height_ (height), objects_ (height, std::vector<Drawable*> (width)) {
    std::srand(std::time(0));
    for (int i = 0; i < objects_.size (); ++i) {
        for (int j = 0; j < objects_[i].size(); ++j) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
                objects_[i][j] = new Wall(i, j);
            }
            else {
                objects_[i][j] = new Floor(i, j);
            }
            if (std::rand() % 101 == 0) {
                Drawable* temp = new Zombie(i, j, 10);
                if (temp->CheckCollision(*objects_[i][j]) == NO_COLLISION) {
                    objects_[i][j] = temp;
                }
                else {
                    delete temp;
                }
            }
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
}

void Game::PutPlayer (int x, int y) {
    delete objects_[x][y];
    player_ = new Knight (x, y, 40);
    objects_[x][y] = player_;
}

void Game::MovePlayer (int dx, int dy) {
    int nx = player_->get_x() + dx;
    int ny = player_->get_y() + dy;
    if (player_->CheckCollision(*objects_[nx][ny]) == NO_COLLISION) {
        Drawable* temp = objects_[nx][ny];
        objects_[nx][ny] = player_;
        objects_[player_->get_x()][player_->get_y()] = temp;
        player_->Move(dx, dy);
        temp->Move(-dx, -dy);
    }
}

void Game::PutPrincess (int x, int y) {
    delete objects_[x][y];
    princess_ = new Princess(x, y);
    objects_[x][y] = princess_;
}

void Game::MoveMonsters() {
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(player_->get_x(), player_->get_y()));
    std::vector<std::vector<bool>> visited(height_, std::vector<bool>(width_, false));
    std::vector<std::vector<int>> distance(height_, std::vector<int>(width_, 0));
    visited[player_->get_x()][player_->get_y()] = true;
    distance[player_->get_x()][player_->get_y()] = 0;
    while (!q.empty()) {
        std::pair<int, int> cur = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = cur.first + dx[i];
            int ny = cur.second + dy[i];
            if (nx < 0 || ny < 0 || nx >= width_ || ny >= height_) {
                continue;
            }
            CollisionType c = objects_[nx][ny]->CheckCollision(*player_);
            if (visited[nx][ny] || distance[cur.first][cur.second] > 11 || c == OBJECT_COLLISION) {
                continue;
            }
            visited[nx][ny] = true;
            q.push(std::make_pair(nx, ny));
            distance[nx][ny] = distance[cur.first][cur.second] + 1;
            if (c == FIGHT_COLLISION) {
                std::swap(objects_[cur.first][cur.second], objects_[nx][ny]);
                objects_[cur.first][cur.second]->Move(nx - cur.first, ny - cur.second);
                objects_[nx][ny]->Move(cur.first - nx, cur.second - ny);
            }
        }

    }
}

void rogue::Game::Render () {
    for (std::vector<Drawable*> i : objects_) {
        for (Drawable* j : i) {
            j->Draw ();
        }
    }
}

}