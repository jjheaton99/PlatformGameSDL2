#pragma once

#include "MeleeObject.h"

class PlayerSideAttack : public MeleeObject
{
public:
    PlayerSideAttack(double xBase, double yBase);

    void update() override;
};

