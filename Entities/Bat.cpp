#include "Bat.h"
#include "Player.h"

Bat::Bat(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int colliderWidth, int colliderHeight, int hitPoints, int damage)
    : FlyingEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, damage)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_dstRect.w = 50;
    m_dstRect.h = 50;
}

void Bat::enemyControls(Character& player)
{
    Vector2D<double> relPos{ player.getPos() + Vector2D<double>{25, 25} - m_position };
    double scaleFactor{m_acceleration / relPos.magnitude()};
    m_velocity.add(scaleFactor * relPos);
    double velMag{ m_velocity.magnitude() };
    if (velMag > m_maxSpeed)
    {
        scaleFactor = m_acceleration / velMag;
        m_velocity.subtract(scaleFactor * m_velocity);
    }
    //std::cout << relPos.direction() << "   " << m_velocity.direction() << '\n';
    //setDirection(relPos.direction());
}