#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Tile.h"
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <tuple>

//base class for characters e.g. players, enemies, npcs
class Character : public GameObject
{
protected:
    bool m_dead{ false };

    Vector2D<double> m_velocity;

    double m_yMaxSpeed{ 0 };
    double m_xMaxSpeed{ 0 };

    std::vector<std::tuple<Collider, double, double>> m_solidColliders;
    std::vector<std::tuple<Collider, double, double>> m_platformColliders;
    bool m_collidingWithLadder{ false };
    double m_ladderxPos{ 0.0 };

    //for checking collisions with map tiles
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) = 0;
    void getCollideTiles(const std::vector<std::vector<Tile>>& map, int characterRow, int characterColumn);
    //for checking collisions with the edge of the map
    virtual bool edgeCheck(const Camera& camera) = 0;

    //for adjusting character velocity
    virtual void motion() = 0;
    virtual void setCollider() { m_collider.setPosition(m_position.getx(), m_position.gety()); }

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    virtual ~Character();

    bool isDead() const { return m_dead; }
    void kill() { m_dead = true; }

    //for player
    virtual void update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<Character*>& enemies) {};
    //for enemies
    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, const Vector2D<double>& playerPos) {};

    const Vector2D<double>& getVel() const { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D<double>{ xVel, yVel }; }
    void setDirection(double angle) { m_velocity.rotate(angle); }
    double getLadderxPos() const { return m_ladderxPos; }
    bool collidingWithLadder() const { return m_collidingWithLadder; }
};

