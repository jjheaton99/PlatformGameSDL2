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

    int m_hitPoints;

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
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0, int hitPoints = 0);
    virtual ~Character();

    //for enemies
    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player) = 0;

    void addHP(int HP) { m_hitPoints += HP; }
    void removeHP(int HP) { m_hitPoints -= HP; }

    bool isDead() const { return m_dead; }
    void kill() { m_dead = true; }

    const Vector2D<double>& getVel() const { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D<double>{ xVel, yVel }; }
    void setVel(Vector2D<double> vel) { m_velocity = vel; }
    void setDirection(double angle);
    void addVel(const Vector2D<double>& vel) { m_velocity.add(vel); }
    void addVel(double x, double y) { m_velocity.add(x, y); }
    void scaleVel(double factor) { m_velocity.scale(factor); }

    double getLadderxPos() const { return m_ladderxPos; }
    bool collidingWithLadder() const { return m_collidingWithLadder; }
};

