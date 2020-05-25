#include "Slime.h"
#include "Player.h"

Slime::Slime(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int hitPoints, int damage)
    : GroundedEnemy(fileName, xStartPos, yStartPos, xVel, yVel, hitPoints, damage)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 9.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 50;
    m_dstRect.h = 50;
}

void Slime::enemyControls(Player& player)
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
}