#pragma once

#include "MeleeObject.h"

class PlayerDownAttack : public MeleeObject
{
public:
    PlayerDownAttack(int damage = 30, double xBase = 0, double yBase = 0);

    void update(std::vector<std::shared_ptr<Character>>& enemies) override;
    //void cameraDraw(const Camera& camera) const override;
};