#include "SpiderShot.h"
#include "Player.h"

SpiderShot::SpiderShot(std::shared_ptr<Character> spider, std::shared_ptr<Player> player)
    : EnemyProjectile(spider, Vector2D<double>{-15.0, -15.0}, player, 0.0, 0.0, 13.33, 13.33, 8, "Assets/Projectiles/spiderShot.png")
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 24 * i, 0, 24, 24 });
    }

    m_dstRect.w = 80;
    m_dstRect.h = 80;

    Vector2D<double> relPos{ player->getPos() + Vector2D<double>{50, 50} - spider->getPos() };
    double scaleFactor{ m_speed / relPos.magnitude() };
    setVel(scaleFactor * relPos);

    m_angle = relPos.direction();
}

void SpiderShot::update(const std::vector<std::vector<Tile>>& map, const Camera& camera)
{
    if (!m_collidedMap && !m_collidedPlayer)
    {
        if (sweepMapCollideCheck(map))
        {
            m_collidedMap = true;
        }

        else if (playerCollideCheck())
        {
            m_collidedPlayer = true;
            m_player->removeHP(m_damage);
        }

        m_position.add(m_velocity);
        setCollider();
    }

    else if (m_collidedPlayer)
    {
        m_position = m_player->getPos() + Vector2D<double>{10.0, 20.0};
        m_player->inflictSlow();
    }

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());

    animateShot();
}

void SpiderShot::animateShot()
{
    if (m_collidedMap || m_collidedPlayer)
    {
        ++m_animationStep;
        if (m_animationStep >= 6)
        {
            m_animationStep = 0;
            ++m_spriteIndex;
            if (m_spriteIndex > 5)
            {
                m_terminated = true;
                m_spriteIndex = 5;
            }
        }
    }

    else
    {
        m_spriteIndex = 0;
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}