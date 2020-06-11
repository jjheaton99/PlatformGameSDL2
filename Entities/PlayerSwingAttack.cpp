#include "PlayerSwingAttack.h"

PlayerSwingAttack::PlayerSwingAttack(int damage, double xBasePos, double yBasePos)
    : MeleeObject("Assets/Attacks/axe.png", damage, xBasePos, yBasePos, 1.0, 1.0, 0.25)
{
    m_srcRect = { 0, 0, 32, 55 };

    m_dstRect.w = 100;
    m_dstRect.h = 172;

    m_offset = Vector2D<double>{ 50.0, 160.0 };

    m_updateAngle = (270.0) / (m_updateCount * (m_updateCount + 1.0));

    m_multiCollider = MultiCollider{ std::vector<Collider::DoubleRect>{
            {0, 0, 5, 5},
            {0, 0, 5, 5},
            {0, 0, 5, 5},
            {0, 0, 5, 5},
            {0, 0, 5, 5},
            {0, 0, 5, 5}
        }
    };
}

void PlayerSwingAttack::rotateColliders(double angle)
{
    for (auto& position : m_colliderOffsets)
    {
        position.rotate(angle);
    }
}

void PlayerSwingAttack::resetColliders()
{
    double reflect{ static_cast<double>(m_facingLeft ? -1.0 : 1.0) };
    m_colliderOffsets = std::vector<Vector2D<double>>{
        Vector2D<double>{reflect * 20.0, - 40},
        Vector2D<double>{reflect * 25.0, - 65},
        Vector2D<double>{reflect * 28.0, - 90},
        Vector2D<double>{reflect * 33.0, - 115},
        Vector2D<double>{reflect * 30.0, - 140 },
        Vector2D<double>{reflect * 20.0, - 160 }
    };
}

bool PlayerSwingAttack::update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel)
{
    updateHitEnemies(enemies);

    bool hit{ false };

    if (m_attacking)
    {
        if (m_counter == 0)
        {
            resetColliders();
            double startAngle{ 45.0 };
            if (m_facingLeft)
            {
                m_angle = startAngle;
                rotateColliders(startAngle);
            }
            else
            {
                m_angle = -startAngle;
                rotateColliders(-startAngle);
            }
        }

        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        if (m_delaying)
        {
            m_collider.setPosition(m_totalPosition);
            m_multiCollider.setPositions(m_position, m_colliderOffsets);

            if (++m_counter > 5)
            {
                m_delaying = false;
                m_counter = 1;
            }
        }

        else if (m_counter < m_updateCount)
        {
            double angle{ m_updateAngle * static_cast<double>(++m_counter) };
            if (m_facingLeft)
            {
                m_angle -= angle;
                rotateColliders(-angle);
            }

            else
            {
                m_angle += angle;
                rotateColliders(angle);
            }

            //still need to set normal collider so it is drawn by cameradraw
            m_collider.setPosition(m_totalPosition);
            m_multiCollider.setPositions(m_position, m_colliderOffsets);

            if (collideCheck(enemies, playerVel, 25.0, 5.0))
            {
                hit = true;
            }
        }

        else
        {
            m_collider.setPosition(m_totalPosition);
            m_multiCollider.setPositions(m_position, m_colliderOffsets);

            /*if (collideCheck(enemies, playerVel, 25.0, 5.0))
            {
                hit = true;
            }*/

            if (++m_counter > m_updateCount + 7)
            {
                cancel();
                m_delaying = true;
            }
        }
    }

    return hit;
}

bool PlayerSwingAttack::collideCheck(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel, double xKnockback, double yKnockback)
{
    bool hit{ false };

    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //if enemy is alive, hasnt been hit by attack and near weapon attack
        if (enemies[i] && !m_hitEnemies[i] && (enemies[i]->getPos() - m_position).magnitude() < 300)
        {
            if (m_multiCollider.collideCheck(enemies[i]->getCollider()))
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

//for testing positions of hitboxes
/*void PlayerSwingAttack::cameraDraw(const Camera& camera) const
{
    SDL_RendererFlip flip{ SDL_FLIP_NONE };
    if (m_facingLeft)
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

        SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
        SDL_Rect rect;
        for (const auto& element : m_multiCollider.getColliders())
        {
            rect = { static_cast<int>(element.getHitBox().x - camera.getx()), static_cast<int>(element.getHitBox().y - camera.gety()), 
                static_cast<int>(element.getHitBox().w), static_cast<int>(element.getHitBox().h) };
            SDL_RenderFillRect(g_renderer, &rect);
        }
    }
}*/
