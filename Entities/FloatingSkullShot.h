#pragma once

#include "EnemyProjectile.h"

class FloatingSkullShot : public EnemyProjectile
{
private:
    double m_acceleration{ 8.0 };
    double m_maxSpeed{ 8.0 };

    void setCollider() override { m_collider.setPosition(m_position.getx() + 3.5, m_position.gety() + 3.5); }
    void motion() override;

public:
    FloatingSkullShot(std::shared_ptr<Character> skull, std::shared_ptr<Player> player);
};

