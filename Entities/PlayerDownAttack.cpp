#include "PlayerDownAttack.h"

PlayerDownAttack::PlayerDownAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/downAttack.png", damage, xBase, yBase, 62.0, 66.0, 0.3)
{
    m_srcRect = { 0, 0, 20, 21 };

    m_dstRect.w = 62;
    m_dstRect.h = 66;

    m_offset = Vector2D<double>{ 31.0, 12.5 };

    m_yoffsetUpdate = 100.0 / m_updateCount;
}

bool PlayerDownAttack::update(std::vector<std::shared_ptr<Character>>& enemies)
{
    updateHitEnemies(enemies);

    bool hit{ false };

    if (m_attacking)
    {
        if (m_counter++ == 0)
        {
            m_offset = Vector2D<double>{ 31.0, 12.5 };
        }

        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        if (m_counter <= m_updateCount / 2)
        {
            m_offset.add(Vector2D<double>{ 0.0, -m_yoffsetUpdate});
            m_offset.add(Vector2D<double>{ 0.0, -m_yoffsetUpdate});

            m_collider.setPosition(m_totalPosition);
            if (collideCheck(enemies, 0.0, 20.0))
            {
                hit = true;
            }
        }

        else if (m_counter <= m_updateCount)
        {
            m_offset.add(Vector2D<double>{ 0.0, m_yoffsetUpdate });
            m_offset.add(Vector2D<double>{ 0.0, m_yoffsetUpdate });

            m_collider.setPosition(m_totalPosition);
            if (collideCheck(enemies, 0.0, 20.0))
            {
                hit = true;
            }
        }

        else
        {
            m_attacking = false;
            m_counter = 0;
            resetHitEnemies();
        }
    }

    return hit;
}