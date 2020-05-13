#pragma once

#include "MeleeObject.h"

class PlayerSideAttack : public MeleeObject
{
private:
    std::vector<Vector2D<double>> m_colliderOffsets;
    MultiCollider m_multiCollider;
    
    void rotateColliders(double angle);
    void resetColliders();

public:
    PlayerSideAttack(double xBase = 0, double yBase = 0);

    void update() override;
    void collideCheck();
    //void cameraDraw(const Camera& camera) const override;
};

