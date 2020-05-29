#include "MeleeObject.h"

MeleeObject::MeleeObject(const char* fileName, int damage, double xBase, double yBase, double colliderWidth, double colliderHeight, double attackDuration)
    : GameObject(fileName, xBase, yBase, colliderWidth, colliderHeight), m_damage{damage}, m_attackDuration{ attackDuration }
{}

MeleeObject::~MeleeObject()
{
    destroy();
}

void MeleeObject::cameraDraw(const Camera& camera) const
{
    if (m_attacking && m_counter != 0)
    {
        SDL_Point rotationPoint{ static_cast<int>(m_offset.getx()), static_cast<int>(m_offset.gety()) };
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, &rotationPoint, SDL_FLIP_NONE);
        }
    }
}

void MeleeObject::updateHitEnemies(const std::vector<std::shared_ptr<Character>>& enemies)
{
    if (m_hitEnemies.size() != enemies.size())
    {
        m_hitEnemies.resize(enemies.size(), false);
    }
}

void MeleeObject::resetHitEnemies()
{
    std::fill(m_hitEnemies.begin(), m_hitEnemies.end(), false);
}