#pragma once

#include "Projectile.h"
#include "Character.h"

class PlayerBoomerang : public Projectile
{
private:
    double m_acceleration{ 5.0 };
    double m_maxSpeed{ 30.0 };
    double m_distanceTravelled{ 0.0 };
    double m_range{ 1000.0 };

    int m_damage;

    bool m_flying{ false };
    bool m_returningToPlayer{ false };

    std::weak_ptr<Character> m_target;

    double m_angle{ 0.0 };

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    bool aquireTargetEnemy(const std::vector<std::shared_ptr<Character>>& enemies);
    bool enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies);

    void motion() override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 15.0, m_position.gety() + 15.0); }

public:
    PlayerBoomerang(double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 10.0, double colliderHeight = 10.0, 
        const char* fileName = "Assets/Attacks/boomerang.png", int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player);
    void cameraDraw(const Camera& camera) const override;

    bool isReturning() const { return m_returningToPlayer; }
    bool isFlying() const { return m_flying; }

    void throwLeft(const std::vector<std::shared_ptr<Character>>& enemies);
    void throwRight(const std::vector<std::shared_ptr<Character>>& enemies);
};

