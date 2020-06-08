#include "MeleeObject.h"

MeleeObject::MeleeObject(const char* fileName, int damage, double xBasePos, double yBasePos, double colliderWidth, double colliderHeight, double attackDuration)
    : GameObject(fileName, xBasePos, yBasePos, colliderWidth, colliderHeight), m_damage{damage}, m_attackDuration{ attackDuration }
{
    m_updateCount = static_cast<int>(m_attackDuration / Constants::updateStep);
}

MeleeObject::~MeleeObject()
{
    destroy();
}

void MeleeObject::cameraDraw(const Camera& camera) const
{
    if (m_attacking && m_counter != 0)
    {
        SDL_RendererFlip flip{ SDL_FLIP_NONE };
        if (m_facingLeft)
        {
            flip = SDL_FLIP_HORIZONTAL;
        }
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

bool MeleeObject::collideCheck(std::vector<std::shared_ptr<Character>>& enemies, double xKnockback, double yKnockback)
{
    bool hit{ false };
    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //if enemy is alive, hasnt been hit by attack and near weapon attack
        if (enemies[i] && !m_hitEnemies[i] && (enemies[i]->getPos() - m_position).magnitude() < 300)
        {
            if (m_collider.collideCheck(enemies[i]->getCollider()))
            {
                hit = true;
                enemies[i]->removeHP(m_damage);
                if (m_facingLeft)
                {
                    enemies[i]->addVel(-xKnockback, -yKnockback);
                }
                else
                {
                    enemies[i]->addVel(xKnockback, -yKnockback);
                }
                m_hitEnemies[i] = true;
            }
        }
    }

    return hit;
}

void MeleeObject::attackLeft()
{ 
    m_facingLeft = true;
    m_attacking = true; 
}

void MeleeObject::attackRight()
{ 
    m_facingLeft = false;
    m_attacking = true; 
}

void MeleeObject::cancel()
{
    m_attacking = false;
    m_counter = 0;
    resetHitEnemies();
}