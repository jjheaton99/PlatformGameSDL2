#include "Player.h"

Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel)
{
    m_xMaxSpeed = 700;

    m_srcRect.w = 100;
    m_srcRect.h = 100;
    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_dstRect.w = 64;
    m_dstRect.h = 64;
}

Player::~Player()
{}

void Player::update(double timeStep)
{
    m_position.add(Vector2D<double>{m_velocity.getx() * timeStep, m_velocity.gety() * timeStep});

    motion();
    edgeCheck();

    std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}