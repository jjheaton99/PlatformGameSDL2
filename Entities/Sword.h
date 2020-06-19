#pragma once
#include "PlayerMeleeAttack.h"

class Sword : public PlayerMeleeAttack
{
private:
    double m_xoffsetUpdate;
    int m_delayCount{ 5 };

    void setCollider();

    SoundEffect m_attackSound{ "Assets/Sounds/PlayerSounds/AttackSounds/light.wav" };

public:
    Sword(int damage = 20, double xBase = 0, double yBase = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel) override;
    //void cameraDraw(const Camera& camera) const override;
};

