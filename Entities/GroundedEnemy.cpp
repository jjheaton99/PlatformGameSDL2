#include "GroundedEnemy.h"

GroundedEnemy::GroundedEnemy(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 100, 100)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 12.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 100;
    m_dstRect.h = 100;
}

void GroundedEnemy::update(const std::vector<std::vector<Tile>>& map, Camera& camera)
{
    m_position.add(m_velocity);
    //collider position is moved after each function that can change character position
    setCollider();

    motion();

    //edge check goes before map collision check to prevent vector subcript error when going off the edge
    if (edgeCheck(camera))
    {
        setCollider();
    }

    mapCollideCheck(map);
    setCollider();

    //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

void GroundedEnemy::enemyControls()
{}

void GroundedEnemy::motion()
{
    switch (m_movement)
    {
    case GroundedCharacter::AIRBORNE:
        if (m_velocity.gety() + Constants::g <= m_yMaxSpeed)
        {
            m_velocity.add(0, Constants::g);
        }
        else
        {
            m_velocity.yScale(0.0);
            m_velocity.add(0, m_yMaxSpeed);
        }
        break;
    case GroundedCharacter::LEFT:
        break;
    case GroundedCharacter::RIGHT:
        break;
    case GroundedCharacter::CLIMB_STOP:
        break;
    case GroundedCharacter::CLIMB_UP:
        break;
    case GroundedCharacter::CLIMB_DOWN:
        break;
    case GroundedCharacter::STOP:
        break;
    default:
        break;
    }
}
