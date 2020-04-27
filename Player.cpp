#include "Player.h"

Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GameObject(fileName, xStartPos, yStartPos), m_velocity{ xVel, yVel }
{
    m_srcRect.w = 100;
    m_srcRect.h = 100;
    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_dstRect.w = 64;
    m_dstRect.h = 64;
}

Player::~Player()
{}

void Player::update()
{
    m_position.add(m_velocity);

    m_velocity.add(Vector2D<double>{0, 0.5});

    //screen edge checking
    if (m_position.getx() < 0)
        m_position.subtract(Vector2D<double>{m_position.getx(), 0});
    else if (m_position.getx() > static_cast<std::int64_t>(Constants::screenWidth) - m_dstRect.w)
        m_position.subtract(Vector2D<double>{m_position.getx() - static_cast<std::int64_t>(Constants::screenWidth) + m_dstRect.w, 0});
    else if (m_position.gety() < 0)
        m_position.subtract(Vector2D<double>{0, m_position.gety()});
    else if (m_position.gety() > static_cast<std::int64_t>(Constants::screenHeight) - m_dstRect.h)
    {
        m_airborne = false;
        m_position.subtract(Vector2D<double>{0, m_position.gety() - static_cast<std::int64_t>(Constants::screenHeight) + m_dstRect.h});
        setVel(m_velocity.getx(), 0);
    }

    std::cout << m_velocity.gety() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

void Player::setVel(double xVel, double yVel)
{
    m_velocity = Vector2D{ xVel, yVel };
}

void Player::setDirection(double angle)
{
    m_velocity.rotate(angle);
}