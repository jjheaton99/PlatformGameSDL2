#include "Player.h"

Player::Player(const char* textureSheet, double xStartPos, double yStartPos, double vx, double vy)
    : GameObject(textureSheet, xStartPos, yStartPos), m_velocity{ vx, vy }
{}

Player::~Player()
{}

void Player::update()
{
    m_position.add(m_velocity);

    m_velocity.add(Vector2D<double>{0, 0.5});

    m_srcRect.w = 100;
    m_srcRect.h = 100;
    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_destRect.x = static_cast<int>(m_position.getx());
    m_destRect.y = static_cast<int>(m_position.gety());
    m_destRect.w = 64;
    m_destRect.h = 64;
}