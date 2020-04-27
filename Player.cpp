#include "Player.h"

Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GameObject(fileName, xStartPos, yStartPos), m_velocity{ xVel, yVel }
{
    m_srcRect.w = 100;
    m_srcRect.h = 100;
    m_srcRect.x = 0;
    m_srcRect.y = 0;
}

Player::~Player()
{}

void Player::update()
{
    m_position.add(m_velocity);

    m_velocity.add(Vector2D<double>{0, 0.5});

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
    m_dstRect.w = 64;
    m_dstRect.h = 64;
}
