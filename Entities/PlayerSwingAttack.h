#pragma once

#include "MeleeObject.h"
#include <vector>

//the player's swinging side melee attack
class PlayerSwingAttack : public MeleeObject
{
private:
    std::vector<Vector2D<double>> m_colliderOffsets;
    MultiCollider m_multiCollider;

    double m_updateAngle;

    void rotateColliders(double angle);
    void resetColliders();

public:
    PlayerSwingAttack(int damage = 30, double xBasePos = 0, double yBasePos = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies) override;
    bool collideCheck(std::vector<std::shared_ptr<Character>>& enemies, double xKnockback, double yKnockback) override;
    //void cameraDraw(const Camera& camera) const override;
};

