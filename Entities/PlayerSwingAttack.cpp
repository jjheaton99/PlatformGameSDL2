#include "PlayerSwingAttack.h"

PlayerSwingAttack::PlayerSwingAttack(int damage, double xBasePos, double yBasePos)
    : MeleeObject("Assets/Attacks/axe.png", damage, xBasePos, yBasePos, 0.0, 0.0, 0.35)
{
    m_srcRect = { 0, 0, 32, 55 };

    m_dstRect.w = 100;
    m_dstRect.h = 172;

    m_offset = Vector2D<double>{ 50.0, 160.0 };

    m_multiCollider = MultiCollider{ std::vector<Collider::DoubleRect>{
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
    m_colliderOffsets = std::vector<Vector2D<double>>{
        Vector2D<double>{0, - 40},
        Vector2D<double>{0, - 65},
        Vector2D<double>{0, - 90},
        Vector2D<double>{0, - 115},
        Vector2D<double>{0, - 140 }
    };
}

void PlayerSwingAttack::update(std::vector<std::shared_ptr<Character>>& enemies)
{
    updateHitEnemies(enemies);

    if (m_attacking)
    {
        if (m_counter == 0)
        {
            resetColliders();
            if (m_facingLeft)
            {
                m_angle = 30.0;
                rotateColliders(30.0);
            }
            else
            {
                m_angle = -30.0;
                rotateColliders(-30.0);
            }
        }

        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        double rotationAngle{ 140.0 / static_cast<int>(m_attackDuration / Constants::updateStep) };
        if (m_facingLeft)
        {
            m_angle -= rotationAngle;
            rotateColliders(-rotationAngle);
        }

        else
        {
            m_angle += rotationAngle;
            rotateColliders(rotationAngle);
        }

        m_collider.setPosition(m_totalPosition);
        m_multiCollider.setPositions(m_position, m_colliderOffsets);

        collideCheck(enemies);

        ++m_counter;
        if (m_counter > static_cast<int>(m_attackDuration / Constants::updateStep))
        {
            m_attacking = false;
            m_counter = 0;
            resetHitEnemies();
        }
    }
}

void PlayerSwingAttack::collideCheck(std::vector<std::shared_ptr<Character>>& enemies)
{
    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //if enemy is alive, hasnt been hit by attack and near weapon attack
        if (enemies[i] && !m_hitEnemies[i] && (enemies[i]->getPos() - m_position).magnitude() < 300)
        {
            if (m_multiCollider.collideCheck(enemies[i]->getCollider()))
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

//for testing positions of hitboxes
/*
void PlayerSwingAttack::cameraDraw(const Camera& camera) const
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
