#pragma once

#include "Projectile.h"
#include "Character.h"

class PlayerBoomerang : public Projectile
{
private:
    int m_damage;

    bool m_returningToPlayer{ false };

    double m_angle{ 0.0 };

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    void enemyCollideCheck(std::vector<std::unique_ptr<Character>>& enemies);

    void motion() override;

public:
    PlayerBoomerang(double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0, const char* fileName = "Assets/Enemies/redSlime.png", int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::unique_ptr<Character>>& enemies);
    void cameraDraw(const Camera& camera) const override;

    bool isReturning() const { return m_returningToPlayer; }
};

