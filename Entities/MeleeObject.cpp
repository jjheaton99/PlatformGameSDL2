#include "MeleeObject.h"

MeleeObject::MeleeObject(const char* fileName, int damage, double xBasePos, double yBasePos, double colliderWidth, double colliderHeight, double attackDuration)
    : GameObject(fileName, xBasePos, yBasePos, colliderWidth, colliderHeight), m_damage{damage}, m_attackDuration{ attackDuration }
{}

MeleeObject::~MeleeObject()
{
    destroy();
}

void MeleeObject::cameraDraw(const Camera& camera) const
{
    SDL_RendererFlip flip{ SDL_FLIP_NONE };
    if (!m_facingLeft)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    if (m_attacking && m_counter != 0)
    {
        SDL_Point rotationPoint{ static_cast<int>(m_offset.getx()), static_cast<int>(m_offset.gety()) };
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, &rotationPoint, flip);
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

void MeleeObject::collideCheck(std::vector<std::shared_ptr<Character>>& enemies)
{
    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //if enemy is alive, hasnt been hit by attack and near weapon attack
        if (enemies[i] && !m_hitEnemies[i] && (enemies[i]->getPos() - m_position).magnitude() < 300)
        {
            if (m_collider.collideCheck(enemies[i]->getCollider()))
            {
                enemies[i]->removeHP(m_damage);
                if (m_facingLeft)
                {
                    enemies[i]->addVel(-20.0, -5.0);
                }
                else
                {
                    enemies[i]->addVel(20.0, -5.0);
                }
                m_hitEnemies[i] = true;
            }
        }
    }
}