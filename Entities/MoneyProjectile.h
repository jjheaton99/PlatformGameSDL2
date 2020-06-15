#pragma once

#include "Projectile.h"
#include "Player.h"

class MoneyProjectile : public Projectile
{
private:
    double m_acceleration{ 2.0 };
    double m_maxSpeed{ 40.0 };

    std::shared_ptr<Player> m_player;

    int m_value{ 50 };

    void motion() override;

public:
    MoneyProjectile(std::shared_ptr<Player> player, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 3.0, double colliderHeight = 3.0,
        std::string fileName = "Assets/MiscTextures/money.png");

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override {};
    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Player> player);
};

