#include "GroundedEnemy.h"

GroundedEnemy::GroundedEnemy(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 50, 50)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 12.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 50;
    m_dstRect.h = 50;
}

void GroundedEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, const Vector2D<double>& playerPos)
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

    enemyControls(playerPos);
    mapCollideCheck(map);
    setCollider();

    //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

void GroundedEnemy::enemyControls(const Vector2D<double>& playerPos)
{
    if (m_position.getx() < playerPos.getx() - m_collider.getHitBox().w)
    {
        m_movement = RIGHT;
    }
    else if (m_position.getx() > playerPos.getx() + 78)
    {
        m_movement = LEFT;
    }
    else
    {
        m_movement = STOP;
    }
}

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
        if (m_velocity.getx() - m_walkAcceleration >= -m_xMaxSpeed)
        {
            m_velocity.add(-m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() < -m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(-m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::RIGHT:
        if (m_velocity.getx() + m_walkAcceleration <= m_xMaxSpeed)
        {
            m_velocity.add(m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() > m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::CLIMB_STOP:
        break;

    case GroundedCharacter::CLIMB_UP:
        break;

    case GroundedCharacter::CLIMB_DOWN:
        break;

    case GroundedCharacter::STOP:
        m_velocity.xScale(0.7);
        if (std::abs(m_velocity.getx()) < 0.0001)
        {
            m_velocity.xScale(0);
        }
        break;

    default:
        break;
    }
}
