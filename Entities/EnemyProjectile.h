#pragma once
#include "Projectile.h"

class Player;

class EnemyProjectile : public Projectile
{
protected:
    std::shared_ptr<Player> m_player;

    virtual bool playerCollideCheck();
    virtual void motion() override = 0;

public:
    EnemyProjectile(std::shared_ptr<Character> enemy, Vector2D<double> relStartPos, std::shared_ptr<Player> player, 
        double xVel = 0.0, double yVel = 0.0, double colliderWidth = 0.0, double colliderHeight = 0.0, int damage = 1, std::string fileName = "Assets/Projectiles/skullShot.png");

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override;
};

