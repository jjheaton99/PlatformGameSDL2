#include "PlayerSideAttack.h"

PlayerSideAttack::PlayerSideAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/Weapons-4.png", damage, xBase, yBase, 50, 50, 0.25)
{
    m_srcRect = { 0, 0, 32, 50 };

    m_dstRect.w = 100;
    m_dstRect.h = 157;

    m_offset = Vector2D<double>{ 50.0, 145.0 };

    m_multiCollider = MultiCollider{ std::vector<Collider::DoubleRect>{
            {0, 0, 8, 8},
            {0, 0, 8, 8},
            {0, 0, 8, 8},
            {0, 0, 8, 8},
            {0, 0, 8, 8},
            {0, 0, 8, 8}
        }
    };
}

void PlayerSideAttack::rotateColliders(double angle)
{
    for (auto& position : m_colliderOffsets)
    {
        position.rotate(angle);
    }
}

void PlayerSideAttack::resetColliders()
{
    m_colliderOffsets = std::vector<Vector2D<double>>{
        Vector2D<double>{0, - 37},
        Vector2D<double>{0, - 55},
        Vector2D<double>{0, - 73},
        Vector2D<double>{0, - 91},
        Vector2D<double>{0, - 109},
        Vector2D<double>{0, - 127 }
    };
}

void PlayerSideAttack::update(std::vector<std::shared_ptr<Character>>& enemies)
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

void PlayerSideAttack::collideCheck(std::vector<std::shared_ptr<Character>>& enemies)
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
void PlayerSideAttack::cameraDraw(const Camera& camera) const
{
    if (m_attacking && m_counter != 0)
    {
        SDL_Point rotationPoint{ static_cast<int>(m_offset.getx()), static_cast<int>(m_offset.gety()) };
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, &rotationPoint, SDL_FLIP_NONE);
        }

        SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
        SDL_Rect rect;
        for (const auto& element : m_multiCollider.getColliders())
        {
            rect = { element.getHitBox().x - camera.getx(), element.getHitBox().y - camera.gety(), element.getHitBox().w, element.getHitBox().h };
            SDL_RenderFillRect(g_renderer, &rect);
        }
    }
}
*/