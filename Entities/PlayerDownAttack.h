#pragma once

#include "MeleeObject.h"

class PlayerDownAttack : public MeleeObject
{
private:
    double m_yoffsetUpdate;

public:
    PlayerDownAttack(int damage = 30, double xBase = 0, double yBase = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies) override;
    //void cameraDraw(const Camera& camera) const override;
};