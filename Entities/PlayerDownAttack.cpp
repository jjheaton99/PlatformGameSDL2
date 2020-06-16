#include "PlayerDownAttack.h"

PlayerDownAttack::PlayerDownAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/downAttack.png", damage, xBase, yBase, 62.0, 66.0, 0.2)
{
    m_itemType = ItemType::DOWN_AXE;

    m_srcRect = { 0, 0, 20, 21 };

    m_dstRect.w = 62;
    m_dstRect.h = 66;

    m_offset = Vector2D<double>{ 31.0, 12.5 };

    m_yoffsetUpdate = 100.0 / m_updateCount;
}

bool PlayerDownAttack::update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel)
{
    updateHitEnemies(enemies);

    bool hit{ false };

    if (m_attacking)
    {
        if (m_counter++ == 0)
        {
            m_offset = Vector2D<double>{ 31.0, 12.5 };
            m_attackSound.play();
        }

        m_totalPosition = m_position - m_offset;
        m_dstRect.x = static_cast<int>(m_totalPosition.getx());
        m_dstRect.y = static_cast<int>(m_totalPosition.gety());

        if (m_counter <= m_updateCount / 2)
        {
            m_offset.add(Vector2D<double>{ 0.0, -m_yoffsetUpdate});
            m_offset.add(Vector2D<double>{ 0.0, -m_yoffsetUpdate});

            m_collider.setPosition(m_totalPosition);
            if (collideCheck(enemies, playerVel, 0.0, 20.0))
            {
                hit = true;
            }
        }

        else if (m_counter <= (m_updateCount / 2) + 5)
        {
            m_collider.setPosition(m_totalPosition);
            if (collideCheck(enemies, playerVel, 0.0, 20.0))
            {
                hit = true;
            }
        }

        else if (m_counter <= m_updateCount + 5)
        {
            m_offset.add(Vector2D<double>{ 0.0, m_yoffsetUpdate });
            m_offset.add(Vector2D<double>{ 0.0, m_yoffsetUpdate });

            m_collider.setPosition(m_totalPosition);
            if (collideCheck(enemies, playerVel, 0.0, 20.0))
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