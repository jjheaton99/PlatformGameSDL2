#pragma once

#include "MeleeObject.h"

class PlayerDownAttack : public MeleeObject
{
private:
    double m_yoffsetUpdate;

public:
    PlayerDownAttack(int damage = 30, double xBase = 0, double yBase = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel) override;
    //void cameraDraw(const Camera& camera) const override;
};