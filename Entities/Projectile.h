#pragma once

#include "GameObject.h"
#include "Character.h"
#include "Tile.h"

class Projectile : public GameObject
{
protected:
    bool m_terminated{ false };
    int m_damage;

    std::vector<Collider::sweptObstacleTuple> m_solidColliders;

    void getCollideTiles(const std::vector<std::vector<Tile>>& map, int projectileRow, int projectileColumn);
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map);
    //virtual bool edgeCheck(const Camera& camera) = 0;

    virtual void setCollider() { m_collider.setPosition(m_position.getx(), m_position.gety()); }
    virtual void motion() = 0;
    
public:
    Projectile(const char* fileName, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0, int damage = 1);
    virtual ~Projectile();

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) = 0;

    bool isTerminated() const { return m_terminated; }
};

