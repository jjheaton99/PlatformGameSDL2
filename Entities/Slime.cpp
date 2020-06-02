#include "Slime.h"
#include "Player.h"

Slime::Slime(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int colliderWidth, int colliderHeight, int hitPoints, int damage)
    : GroundedEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, damage)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 9.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 50;
    m_dstRect.h = 50;
}

void Slime::enemyControls(Character& player)
{
    if (m_movement != AIRBORNE)
    {
        if (m_position.getx() < player.getPos().getx() + 20 - m_collider.getHitBox().w)
        {
            m_movement = RIGHT;
        }
        else if (m_position.getx() > player.getPos().getx() + 80)
        {
            m_movement = LEFT;
        }
        else
        {
            m_movement = STOP;
        }

        if (MTRandom::getRandomInt(0, 99) < 2)
        {
            m_movement = AIRBORNE;
            addVel(Vector2D<double>{0.0, -20.0});
        }
    }
}