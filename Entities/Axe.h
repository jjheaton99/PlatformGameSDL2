#pragma once

#include "PlayerMeleeAttack.h"
#include <vector>

//the player's swinging side melee attack
class Axe : public PlayerMeleeAttack
{
private:
    std::vector<Vector2D<double>> m_colliderOffsets;
    MultiCollider m_multiCollider;

    double m_updateAngle;
    bool m_delaying{ true };

    void rotateColliders(double angle);
    void resetColliders();

    SoundEffect m_attackSound{ "Assets/Sounds/PlayerSounds/AttackSounds/axe.wav" };

public:
    Axe();

    bool update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel) override;
    bool collideCheck(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel, double xKnockback, double yKnockback) override;
    //void cameraDraw(const Camera& camera) const override;
};

