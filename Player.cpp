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
    
    m_spriteIndex = 0;

    m_dstRect.w = 100;
    m_dstRect.h = 100;
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

    spriteAnimate(timeStep);
}

void Player::cycleWalkAnimation(double timeStep)
{
    m_animationTime += timeStep;
    if (m_animationTime > timeBetweenSpriteFrames)
    {
        m_animationTime = 0.0;

        ++m_spriteIndex;
        if (m_spriteIndex > 26)
        {
            m_spriteIndex = 5;
        }
    }
}

void Player::cycleIdleAnimation(double timeStep)
{
    m_animationTime += timeStep;
    if (m_animationTime > (timeBetweenSpriteFrames * 3.0))
    {
        m_animationTime = 0.0;

        ++m_spriteIndex;
        if (m_spriteIndex > 4)
        {
            m_spriteIndex = 0;
        }
    }
}

void Player::spriteAnimate(double timeStep)
{
    if (m_movement == AIRBORNE)
    {
        m_spriteIndex = 27;
        m_animationTime = 0.0;
    }

    else if (m_movement == LEFT)
    {
        if (m_isFlipped == false)
        {
            m_isFlipped = true;
            m_spriteIndex = 5;
        }
        
        cycleWalkAnimation(timeStep);
    }

    else if (m_movement == RIGHT)
    {
        if (m_isFlipped == true)
        {
            m_isFlipped = false;
            m_spriteIndex = 5;
        }

        cycleWalkAnimation(timeStep);
    }

    else
    {
        cycleIdleAnimation(timeStep);
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
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