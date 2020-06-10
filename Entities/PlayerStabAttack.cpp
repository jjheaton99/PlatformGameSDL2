#include "PlayerStabAttack.h"

PlayerStabAttack::PlayerStabAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/sword3.png", damage, xBase, yBase, 131.0, 22.0, 0.16)
{
    m_srcRect = { 0, 0, 42, 15 };

    m_dstRect.w = 131;
    m_dstRect.h = 47;

    m_offset = Vector2D<double>{ 20.0, 22.0 };

    m_xoffsetUpdate = 150.0 / m_updateCount;
}

bool PlayerStabAttack::update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel)
{
    updateHitEnemies(enemies);

    bool hit{ false };

    if (m_attacking)
    {
        if (m_counter++ == 0)
        {
            if (m_facingLeft)
            {
                m_offset = Vector2D<double>{ 111.0, 22.0 };
            }
            else
            {
                m_offset = Vector2D<double>{ 20.0, 22.0 };
            }
        }

        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        if (m_counter <= m_delayCount)
        {
            setCollider();
            if (collideCheck(enemies, playerVel, 0.0, 0.0))
            {
                hit = true;
            }
        }

        else if (m_counter <= (m_updateCount / 2) + m_delayCount)
        {
            if (m_facingLeft)
            {
                m_offset.add(Vector2D<double>{ m_xoffsetUpdate, 0.0 });
            }
            else
            {
                m_offset.add(Vector2D<double>{ -m_xoffsetUpdate, 0.0 });
            }

            setCollider();
            if (collideCheck(enemies, playerVel, 10.0, 0.0))
            {
                hit = true;
            }
        }

        else if (m_counter <= m_updateCount + m_delayCount)
        {
            if (m_facingLeft)
            {
                m_offset.add(Vector2D<double>{ -m_xoffsetUpdate, 0.0 });
            }
            else
            {
                m_offset.add(Vector2D<double>{ m_xoffsetUpdate, 0.0 });
            }

            setCollider();
            if (collideCheck(enemies, playerVel, 10.0, 0.0))
            {
                hit = true;
            }
        }

        else
        {
            cancel();
        }
    }

    return hit;
}

void PlayerStabAttack::setCollider()
{
    if (m_facingLeft)
    {
        m_collider.setPosition(m_totalPosition.getx(), m_totalPosition.gety() + 12.5);
    }
    else
    {
        m_collider.setPosition(m_totalPosition.getx(), m_totalPosition.gety() + 12.5);
    }
}