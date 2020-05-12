#include "PlayerSideAttack.h"

PlayerSideAttack::PlayerSideAttack(double xBase, double yBase)
    : MeleeObject("Assets/Attacks/Weapons-4.png", xBase, yBase, 50, 50, 0.3)
{
    m_srcRect = { 0, 0, 32, 50 };

    m_dstRect.w = 100;
    m_dstRect.h = 157;

    m_offset = Vector2D<double>{ 50.0, 150.0 };
}

void PlayerSideAttack::update()
{
    if (m_attacking)
    {
        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        double rotationAngle{ 140.0 / static_cast<int>(m_attackDuration / Constants::updateStep) };
        if (m_facingLeft)
        {
            m_angle -= rotationAngle;
        }

        else
        {
            m_angle += rotationAngle;
        }

        m_collider.setPosition(static_cast<int>(m_totalPosition.getx() + 22), static_cast<int>(m_totalPosition.gety()));

        ++m_counter;
        if (m_counter > static_cast<int>(m_attackDuration / Constants::updateStep))
        {
            m_attacking = false;
            m_counter = 0;
        }
    }
}
