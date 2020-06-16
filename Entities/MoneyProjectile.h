#pragma once

#include "Projectile.h"
#include "Player.h"

class MoneyProjectile : public Projectile
{
private:
    double m_acceleration{ 3.0 };
    double m_maxSpeed{ 15.0 };
    int m_homingDelayCount{ 0 };
    int m_homingDelay;

    std::shared_ptr<Player> m_player;

    int m_value;

    void motion() override;

public:
    MoneyProjectile(std::shared_ptr<Player> player, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 10.0, double colliderHeight = 10.0,
        std::string fileName = "Assets/MiscTextures/money.png");

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override;
};

