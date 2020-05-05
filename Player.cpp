#include "Player.h"

//Player collision box width = 0.56 * collision box height
Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, 56, 100)
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

    m_dstRect.w = m_collider.getBox().h;
    m_dstRect.h = m_collider.getBox().h;

    m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));
}

Player::~Player()
{
    destroy();
}

void Player::update(double timeStep, std::vector<std::vector<Tile>>& map, Camera& camera)
{
    Vector2D<double> timeScaledVel{ Vector2D<double>{m_velocity.getx()* timeStep, m_velocity.gety()* timeStep} };
    m_position.add(timeScaledVel);
    
    mapCollideCheck(map);
    edgeCheck(camera);
    m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));

    motion(30, 0.9, m_maxVel);

    //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());

    spriteAnimate(timeStep);
    moveCamera(camera);
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

void Player::cameraDraw(Camera& camera)
{   
    SDL_RendererFlip flip{ SDL_FLIP_NONE };
    if (m_isFlipped)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, 0, nullptr, flip);
    }
}

void Player::moveCamera(Camera& camera)
{
    camera.setPos(static_cast<int>(m_position.getx()) - (Constants::screenWidth / 2),
        static_cast<int>(m_position.gety()) - (Constants::screenHeight / 2));

    //Now to boundary check the camera
    if (!camera.xInBoundary() && !camera.yInBoundary())
    {
        if (camera.getx() < 0 && camera.gety() < 0)
        {
            camera.setPos(0, 0);
        }

        else if (camera.getx() > camera.getxMax() && camera.gety() < 0)
        {
            camera.setPos(camera.getxMax(), 0);
        }

        else if (camera.getx() < 0 && camera.gety() > camera.getyMax())
        {
            camera.setPos(0, camera.getyMax());
        }

        else if (camera.getx() > camera.getxMax() && camera.gety() > camera.getyMax())
        {
            camera.setPos(camera.getxMax(), camera.getyMax());
        }
    }

    else if (!camera.xInBoundary() && camera.yInBoundary())
    {
        if (camera.getx() < 0)
        {
            camera.setPos(0, camera.gety());
        }

        else if (camera.getx() > camera.getxMax())
        {
            camera.setPos(camera.getxMax(), camera.gety());
        }
    }

    else if (camera.xInBoundary() && !camera.yInBoundary())
    {
        if (camera.gety() < 0)
        {
            camera.setPos(camera.getx(), 0);
        }

        else if (camera.gety() > camera.getyMax())
        {
            camera.setPos(camera.getx(), camera.getyMax());
        }
    }
}