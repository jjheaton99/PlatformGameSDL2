#include "Bat.h"
#include "Player.h"

Bat::Bat(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : FlyingEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount, damage)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 32 * i, 0, 32, 32 });
    }

    m_dstRect.w = 80;
    m_dstRect.h = 80;
}

void Bat::enemyControls(const std::shared_ptr<Character> player)
{
    Vector2D<double> relPos{ player->getPos() + Vector2D<double>{25, 25} - m_position };
    double scaleFactor{m_acceleration / relPos.magnitude()};
    m_velocity.add(scaleFactor * relPos);
    double velMag{ m_velocity.magnitude() };

    //remove excess speed
    if (velMag > m_maxSpeed)
    {
        scaleFactor = (velMag - m_maxSpeed) / velMag;
        m_velocity.subtract(scaleFactor * m_velocity);
    }

    if (m_velocity.getx() > 0)
    {
        m_facingLeft = false;
    }
    else
    {
        m_facingLeft = true;
    }
}

void Bat::animateSprite()
{
    ++m_animationStep;
    if (m_animationStep >= 6)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 4)
        {
            m_spriteIndex = 0;
        }
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}