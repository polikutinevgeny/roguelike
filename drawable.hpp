#pragma once

namespace rogue {

class Drawable;
class Moveable;
class Character;
class Fightable;
class Monster;
class Princess;
class Knight;
class Zombie;
class Dragon;
class Wall;
class Floor;


enum CollisionType {
    NO_COLLISION = 0,
    FIGHT_COLLISION = 1,
    OBJECT_COLLISION = 2,
    NO_FIGHT_COLLISION = 3,
    ERROR_COLLISION = 4,
    PRINCESS_COLLISION = 5,
};

class Drawable {

public:
    Drawable(int x, int y) : x_(x), y_(y) {};

    virtual void Draw() = 0;

    virtual CollisionType CheckCollision(Drawable& other) = 0;
    virtual CollisionType CheckCollision(Knight& other) = 0;
    virtual CollisionType CheckCollision(Princess& other) = 0;
    virtual CollisionType CheckCollision(Monster& other) = 0;
    virtual CollisionType CheckCollision(Wall& other) = 0;
    virtual CollisionType CheckCollision(Floor& other) = 0;

protected:
    int x_;
    int y_;

};

class Moveable : public Drawable {

public:
    Moveable(int x, int y) : Drawable(x, y) {};
    virtual void Move(int dx, int dy) = 0;

};

class Character : public Moveable {

public:
    Character(int x, int y) : Moveable(x, y) {};

};

class Fightable : public Character {

public:
    Fightable(int x, int y, int damage) : Character(x, y), damage_(damage) {};

    inline unsigned int get_hp() const {
        return hit_points_;
    }
    inline void set_hp(const int hp) {
        hit_points_ = hp;
    }
    inline int get_damage() const {
        return damage_;
    }

    virtual void Hit() = 0;

private:
    unsigned int hit_points_;
    const int damage_;

};

class Monster : public Fightable {

public:
    Monster(int x, int y, int damage) : Fightable(x, y, damage) {};

    CollisionType CheckCollision (Drawable& other) override;
    CollisionType CheckCollision (Knight& other) override;
    CollisionType CheckCollision (Princess& other) override;
    CollisionType CheckCollision (Monster& other) override;
    CollisionType CheckCollision (Wall& other) override;
    CollisionType CheckCollision (Floor& other) override;

};

class Princess : public Character {

public:
    Princess(int x, int y) : Character(x, y) {};

    void Draw() override;

    void Move(int dx, int dy) override;

    CollisionType CheckCollision (Drawable& other) override;
    CollisionType CheckCollision (Knight& other) override;
    CollisionType CheckCollision (Princess& other) override;
    CollisionType CheckCollision (Monster& other) override;
    CollisionType CheckCollision (Wall& other) override;
    CollisionType CheckCollision (Floor& other) override;

};

class Knight : public Fightable {

public:
    Knight(int x, int y, int damage) : Fightable(x, y, damage) {};

    void Draw() override;

    void Move(int dx, int dy) override;

    void Hit() override;

    CollisionType CheckCollision (Drawable& other) override;
    CollisionType CheckCollision (Knight& other) override;
    CollisionType CheckCollision (Princess& other) override;
    CollisionType CheckCollision (Monster& other) override;
    CollisionType CheckCollision (Wall& other) override;
    CollisionType CheckCollision (Floor& other) override;

};

class Zombie : public Monster {

public:
    Zombie(int x, int y, int damage) : Monster(x, y, damage) {};

    void Draw() override;

    void Move(int dx, int dy) override;

    void Hit() override;

};

class Dragon : public Monster {

public:
    Dragon(int x, int y, int damage) : Monster(x, y, damage) {};

    void Draw() override;

    void Move(int dx, int dy) override;

    void Hit() override;

};

class Wall : public Drawable {

public:
    Wall(int x, int y) : Drawable(x, y) {};

    void Draw() override;

    CollisionType CheckCollision (Drawable& other) override;
    CollisionType CheckCollision (Knight& other) override;
    CollisionType CheckCollision (Princess& other) override;
    CollisionType CheckCollision (Monster& other) override;
    CollisionType CheckCollision (Wall& other) override;
    CollisionType CheckCollision (Floor& other) override;

};

class Floor : public Drawable {

public:
    Floor(int x, int y) : Drawable(x, y) {};

    void Draw() override;

    CollisionType CheckCollision (Drawable& other) override;
    CollisionType CheckCollision (Knight& other) override;
    CollisionType CheckCollision (Princess& other) override;
    CollisionType CheckCollision (Monster& other) override;
    CollisionType CheckCollision (Wall& other) override;
    CollisionType CheckCollision (Floor& other) override;

};

}