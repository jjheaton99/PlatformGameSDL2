#include "FloatingSkullShot.h"
#include "Player.h"

FloatingSkullShot::FloatingSkullShot(std::shared_ptr<Character> skull, std::shared_ptr<Player> player)
    : EnemyProjectile(skull, Vector2D<double>{56.0, 56.0}, player, 0.0, 0.0, 21, 21, 25, "Assets/Projectiles/skullShot.png")
{
    m_srcRect = { 0, 0, 8, 8 };

    m_dstRect.w = 28;
    m_dstRect.h = 28;
}

void FloatingSkullShot::motion()
{
    double scaleFactor{};
    double velMag{};
    if (m_player)
    {
        Vector2D<double> relPos{ m_player->getPos() + Vector2D<double>{50.0, 50.0} - m_position };
        scaleFactor = m_acceleration / relPos.magnitude();
        m_velocity.add(scaleFactor * relPos);
        velMag = m_velocity.magnitude();

        if (velMag > m_maxSpeed)
        {
            //subtract excess speed
            scaleFactor = (velMag - m_maxSpeed) / velMag;
            m_velocity.subtract(scaleFactor * m_velocity);
        }
    }
}