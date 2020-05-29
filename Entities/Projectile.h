#pragma once

#include "GameObject.h"
#include "Character.h"
#include "Tile.h"

class Projectile : public GameObject
{
protected:
    bool m_terminated{ false };

    std::vector<Collider::sweptObstacleTuple> m_solidColliders;

    void getCollideTiles(const std::vector<std::vector<Tile>>& map, int projectileRow, int projectileColumn);
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) = 0;
    //virtual bool edgeCheck(const Camera& camera) = 0;

    virtual void motion() = 0;
    
public:
    Projectile(const char* fileName, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0);
    virtual ~Projectile();

    bool isTerminated() const { return m_terminated; }
};

