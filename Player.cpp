#include "Player.h"

Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel)
{
    for (int i{ 0 }; i < spriteSheetCount; ++i)
    {
        m_spriteRects[i].w = 32;
        m_spriteRects[i].h = 32;
        m_spriteRects[i].x = i * 32;
        m_spriteRects[i].y = 0;
    }

    m_xMaxSpeed = 700;
    
    m_srcRect = m_spriteRects[0];

    m_dstRect.w = 64;
    m_dstRect.h = 64;
}

Player::~Player()
{}

void Player::update(double timeStep)
{
    m_position.add(Vector2D<double>{m_velocity.getx() * timeStep, m_velocity.gety() * timeStep});

    motion(30, 0.9);
    edgeCheck();

    std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

void Player::draw()
{   
    SDL_RendererFlip flip{ SDL_FLIP_NONE };
    if (m_isFlipped)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    m_texture.draw(m_srcRect, m_dstRect, 0, nullptr, flip);
}