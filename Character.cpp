#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GameObject(fileName, xStartPos, yStartPos), m_velocity{ xVel, yVel }
{}

Character::~Character()
{}

void Character::setVel(double xVel, double yVel)
{
    m_velocity = Vector2D{ xVel, yVel };
}

void Character::setDirection(double angle)
{
    m_velocity.rotate(angle);
}

void Character::moveLeft()
{
    m_left = true;
    m_right = false;
}

void Character::moveRight()
{
    m_right = true;
    m_left = false;
}

void Character::stop()
{
    m_left = false;
    m_right = false;
}

void Character::motion(double acceleration, double deceleration)
{
    if (m_airborne)
    {
        m_velocity.add(Vector2D<double>{0, Constants::g});
    }
    else if (m_left)
    {
        m_velocity.add(Vector2D<double>{-acceleration, 0});
        if (m_velocity.getx() < -m_xMaxSpeed)
        {
            setVel(-m_xMaxSpeed, 0);
        }
    }
    else if (m_right)
    {
        m_velocity.add(Vector2D<double>{acceleration, 0});
        if (m_velocity.getx() > m_xMaxSpeed)
        {
            setVel(m_xMaxSpeed, 0);
        }
    }
    else
    {
        m_velocity.xScale(deceleration);
        if (std::abs(m_velocity.getx()) < 0.1)
        {
            m_velocity.xScale(0);
        }
    }
}

void Character::edgeCheck()
{
    if (m_position.getx() < 0)
    {
        m_position.subtract(Vector2D<double>{m_position.getx(), 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.getx() > static_cast<std::int64_t>(Constants::screenWidth) - m_dstRect.w)
    {
        m_position.subtract(Vector2D<double>{m_position.getx() - static_cast<std::int64_t>(Constants::screenWidth) + m_dstRect.w, 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.gety() < 0)
    {
        m_position.subtract(Vector2D<double>{0, m_position.gety()});
    }
    else if (m_position.gety() > static_cast<std::int64_t>(Constants::screenHeight) - m_dstRect.h)
    {
        m_airborne = false;
        m_position.subtract(Vector2D<double>{0, m_position.gety() - static_cast<std::int64_t>(Constants::screenHeight) + m_dstRect.h});
        setVel(m_velocity.getx(), 0);
    }
}