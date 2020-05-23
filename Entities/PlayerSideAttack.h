#pragma once

#include "MeleeObject.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include <vector>

//the player's swinging side melee attack
class PlayerSideAttack : public MeleeObject
{
private:
    std::vector<Vector2D<double>> m_colliderOffsets;
    MultiCollider m_multiCollider;
    
    void rotateColliders(double angle);
    void resetColliders();

public:
    PlayerSideAttack(int damage = 1, double xBase = 0, double yBase = 0);

    void update(std::vector<std::unique_ptr<GroundedEnemy>>& enemies);
    void collideCheck(std::vector<std::unique_ptr<GroundedEnemy>>& enemies);
    //void cameraDraw(const Camera& camera) const override;
};

