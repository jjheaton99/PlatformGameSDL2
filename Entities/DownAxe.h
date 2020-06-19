#pragma once

#include "PlayerMeleeAttack.h"

class DownAxe : public PlayerMeleeAttack
{
private:
    double m_yoffsetUpdate;

    SoundEffect m_attackSound{ "Assets/Sounds/PlayerSounds/AttackSounds/light.wav" };

public:
    DownAxe(int damage = 30, double xBase = 0, double yBase = 0);

    bool update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel) override;
    //void cameraDraw(const Camera& camera) const override;
};