#pragma once
#include "Projectile.h"

class EnemyProjectile : public Projectile
{
protected:
    std::weak_ptr<Character> m_player;

    virtual bool playerCollideCheck();
    virtual void motion() override = 0;

public:
    EnemyProjectile(std::shared_ptr<Character> enemy, Vector2D<double> relStartPos, std::shared_ptr<Character> player, 
        double xVel = 0.0, double yVel = 0.0, double colliderWidth = 0.0, double colliderHeight = 0.0, int damage = 1, const char* fileName = "Assets/Projectiles/skullShot.png");

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override;
};

