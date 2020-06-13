#include "MeleeObject.h"

MeleeObject::MeleeObject(std::string fileName, int damage, double xBasePos, double yBasePos, double colliderWidth, double colliderHeight, double attackDuration)
    : GameObject(fileName, xBasePos, yBasePos, 0.0, 0.0, colliderWidth, colliderHeight), m_damage{damage}, m_attackDuration{ attackDuration }
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
        //for testing hitbox
        /*SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
        SDL_Rect rect;
        rect = { static_cast<int>(m_collider.getHitBox().x - camera.getx()), static_cast<int>(m_collider.getHitBox().y - camera.gety()),
        static_cast<int>(m_collider.getHitBox().w), static_cast<int>(m_collider.getHitBox().h) };
        SDL_RenderFillRect(g_renderer, &rect);*/
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

bool MeleeObject::collideCheck(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel, double xKnockback, double yKnockback)
{
    bool attackLanded{ false };
    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //if enemy is alive, hasnt been hit by attack and near weapon attack
        if (enemies[i] && !m_hitEnemies[i] && (enemies[i]->getPos() - m_position).magnitude() < 1000)
        {
            bool hit{ false };
            if (m_collider.collideCheck(enemies[i]->getCollider()))
            {
                hit = true;
            }
            else
            {
                Collider::sweptObstacleTuple sweptCollider{ enemies[i]->getCollider(), Collider::xOverlap(m_collider, enemies[i]->getCollider()),
                    Collider::yOverlap(m_collider, enemies[i]->getCollider()) };
                if (m_collider.sweptAABBCheck(playerVel, enemies[i]->getVel(), sweptCollider).first != Collider::NONE)
                {
                    hit = true;
                }
            }

            if (hit)
            {
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
                attackLanded = true;
            }
        }
    }
    return attackLanded;
}

void MeleeObject::attackLeft()
{ 
    if (!m_attacking)
    {
        m_facingLeft = true;
        m_attacking = true;
    }
}

void MeleeObject::attackRight()
{ 
    if (!m_attacking)
    {
        m_facingLeft = false;
        m_attacking = true;
    }
}

void MeleeObject::cancel()
{
    m_attacking = false;
    m_counter = 0;
    resetHitEnemies();
}