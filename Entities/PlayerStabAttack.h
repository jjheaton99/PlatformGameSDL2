#pragma once
#include "MeleeObject.h"

class PlayerStabAttack : public MeleeObject
{
private:
    double m_xoffsetUpdate;

    void setCollider();

public:
    PlayerStabAttack(int damage = 10, double xBase = 0, double yBase = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies) override;
    //void cameraDraw(const Camera& camera) const override;
};

