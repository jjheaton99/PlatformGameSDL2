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
    const int m_spriteSheetCount;
    int m_animationStep{ 0 };
    std::vector<SDL_Rect> m_spriteRects;
    std::vector<SDL_Rect>::size_type m_spriteIndex{ 0 };

    bool m_dead{ false };

    double m_yMaxSpeed{ 0 };
    double m_xMaxSpeed{ 0 };

    bool m_facingLeft{ false };

    int m_maxHitPoints;
    int m_hitPoints;

    int m_damageFlashCount{ 6 };
    void cycleDamageFlash();

    std::vector<Collider::sweptObstacleTuple> m_solidColliders;
    std::vector<Collider::sweptObstacleTuple> m_platformColliders;
    std::vector<Collider::sweptObstacleTuple> m_spikeColliders;
    bool m_collidingWithLadder{ false };
    double m_ladderxPos{ 0.0 };

    ProjectileType m_projectile{ ProjectileType::NONE };

    //for checking collisions with map tiles
    void getCollideTiles(const std::vector<std::vector<Tile>>& map, int characterRow, int characterColumn);
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) = 0;
    //for checking collisions with the edge of the map
    virtual bool edgeCheck(const Camera& camera) = 0;

    //for adjusting character velocity
    virtual void motion() = 0;
    virtual void animateSprite() = 0;

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, 
        double colliderWidth = 0, double colliderHeight = 0, int hitPoints = 0, int spriteSheetCount = 1);
    virtual ~Character();

    //for enemies
    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) = 0;
    virtual void cameraDraw(const Camera& camera) const override;

    void addHP(int HP);
    virtual void removeHP(int HP);
    int getMaxHP() const { return m_maxHitPoints; }
    int getHP() const { return m_hitPoints; }

    bool isDead() const { return m_dead; }
    void kill() { m_dead = true; }

    double getLadderxPos() const { return m_ladderxPos; }
    bool collidingWithLadder() const { return m_collidingWithLadder; }

    void faceLeft() { m_facingLeft = true; }
    void faceRight() { m_facingLeft = false; }

    GameObject::ProjectileType getProjectile() const { return m_projectile; }
};

