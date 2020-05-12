#pragma once

#include "MeleeObject.h"

class PlayerSideAttack : public MeleeObject
{
public:
    PlayerSideAttack(double xBase, double yBase);

    void update() override;
    void cancel() { m_counter = static_cast<int>(m_attackDuration / Constants::updateStep) + 1; }
};

