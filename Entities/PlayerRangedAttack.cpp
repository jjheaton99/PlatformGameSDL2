#include "PlayerRangedAttack.h"

PlayerRangedAttack::PlayerRangedAttack(std::string fileName, double xPos, double yPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int damage, double cooldown)
    : Projectile(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight, damage), m_coolDown{cooldown}
{}

void PlayerRangedAttack::cameraDraw(const Camera& camera) const
{
    if (m_flying)
    {
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, SDL_FLIP_NONE);
        }
    }
}

void PlayerRangedAttack::shootLeft()
{
    if (!m_isCooling && !m_flying)
    {
        m_shootLeft = true;
    }
}

void PlayerRangedAttack::shootRight()
{
    if (!m_isCooling && !m_flying)
    {
        m_shootRight = true;
    }
}

double PlayerRangedAttack::getCooldownFraction() const
{
    if (m_flying)
    {
        return 1.0;
    }
    else if (!m_isCooling)
    {
        return 0.0;
    }
    else
    {
        return 1.0 - (static_cast<double>(m_coolDownCount) / (m_coolDown / Constants::updateStep));
    }
}