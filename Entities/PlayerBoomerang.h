#pragma once

#include "Projectile.h"
#include "Character.h"

class PlayerBoomerang : public Projectile
{
private:
    ItemType m_itemType{ ItemType::BOOMERANG };

    double m_acceleration{ 50.0 };
    double m_maxSpeed{ 20.0 };
    double m_distanceTravelled{ 0.0 };
    double m_range{ 1000.0 };
    int m_maxCollisions{ 3 };
    int m_collisionCount{ 0 };

    bool m_throwLeft{ false };
    bool m_throwRight{ false };
    bool m_flying{ false };
    bool m_returningToPlayer{ false };
    double m_coolDown{ 3.0 };
    int m_coolDownCount{ 0 };
    bool m_isCooling{ false };

    std::weak_ptr<Character> m_target;
    std::weak_ptr<Character> m_prevTarget;

    double m_angle{ 0.0 };
    int m_flyingSoundCount{ 0 };

    SoundEffect m_hitWallSound{ "Assets/Sounds/PlayerSounds/land.wav" };
    SoundEffect m_flyingSound{ "Assets/Sounds/PlayerSounds/AttackSounds/boomerang.wav" };

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    bool aquireTargetEnemy(const std::vector<std::shared_ptr<Character>>& enemies);
    bool enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies);
    void returnToPlayer(std::shared_ptr<Character> player);

    void motion() override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 15.0, m_position.gety() + 15.0); }

public:
    PlayerBoomerang(double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 10.0, double colliderHeight = 10.0, 
        std::string fileName = "Assets/Attacks/boomerang.png", int damage = 20);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override {};
    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player);
    void cameraDraw(const Camera& camera) const override;

    bool isReturning() const { return m_returningToPlayer; }
    bool isFlying() const { return m_flying; }

    void throwLeft();
    void throwRight();

    double getCooldownFraction() const;
};

