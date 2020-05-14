#pragma once

#include "MeleeObject.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include <vector>

class PlayerSideAttack : public MeleeObject
{
private:
    std::vector<Vector2D<double>> m_colliderOffsets;
    MultiCollider m_multiCollider;
    
    void rotateColliders(double angle);
    void resetColliders();

public:
    PlayerSideAttack(double xBase = 0, double yBase = 0);

    void update() {};
    void update(std::vector<Character*>& enemies);
    void collideCheck(std::vector<Character*>& enemies);
    //void cameraDraw(const Camera& camera) const override;
};

