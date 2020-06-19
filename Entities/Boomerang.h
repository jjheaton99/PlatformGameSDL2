#pragma once

#include "PlayerRangedAttack.h"

class Boomerang : public PlayerRangedAttack
{
private:
    double m_acceleration{ 50.0 };
    double m_maxSpeed{ 20.0 };
    double m_distanceTravelled{ 0.0 };
    double m_range{ 1000.0 };
    int m_maxCollisions{ 3 };
    int m_collisionCount{ 0 };

    bool m_returningToPlayer{ false };

    std::weak_ptr<Character> m_target;
    std::weak_ptr<Character> m_prevTarget;

    int m_flyingSoundCount{ 0 };

    SoundEffect m_hitWallSound{ "Assets/Sounds/PlayerSounds/land.wav" };
    SoundEffect m_flyingSound{ "Assets/Sounds/PlayerSounds/AttackSounds/boomerang.wav" };

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    bool aquireTargetEnemy(const std::vector<std::shared_ptr<Character>>& enemies);
    bool enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies) override;
    void returnToPlayer(std::shared_ptr<Character> player);

    void motion() override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 15.0, m_position.gety() + 15.0); }

public:
    Boomerang();

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player) override;

    bool isReturning() const { return m_returningToPlayer; }
    bool isFlying() const { return m_flying; }
};

