#include "Player.h"

Player::Player(const char* textureSheet, int xStartPos, int yStartPos)
    : GameObject(textureSheet, xStartPos, yStartPos)
{}

Player::~Player()
{}

void Player::update()
{
    m_xpos++;
    m_ypos++;

    m_srcRect.w = 100;
    m_srcRect.h = 100;
    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_destRect.x = m_xpos;
    m_destRect.y = m_ypos;
    m_destRect.w = 64;
    m_destRect.h = 64;
}