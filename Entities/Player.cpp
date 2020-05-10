#include "Player.h"

//Player collision hitBox width = 0.56 * collision hitBox height
Player::Player(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 56, 100)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects[i].w = 32;
        m_spriteRects[i].h = 32;
        m_spriteRects[i].x = i * 32;
        m_spriteRects[i].y = 0;
    }

    m_yMaxSpeed = 25.0;
    m_xMaxSpeed = 12.0;
    m_walkAcceleration = 0.8;
    
    m_spriteIndex = 0;

    m_dstRect.w = m_collider.getHitBox().h;
    m_dstRect.h = m_collider.getHitBox().h;

    m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));
}

Player::~Player()
{
    destroy();
}

void Player::update(std::vector<std::vector<Tile>>& map, Camera& camera)
{
    Vector2D<double> timeScaledVel{ Vector2D<double>{m_velocity.getx(), m_velocity.gety()} };
    m_position.add(timeScaledVel);
    //collider position is moved after each function that can change character position
    m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));

    motion();

    //edge check goes before map collision check to prevent vector subcript error when going off the edge
    if (edgeCheck(camera))
    {
        m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));
    }

    mapCollideCheck(map);
    m_collider.setPosition(static_cast<int>(m_position.getx() + 22), static_cast<int>(m_position.gety()));

    std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());

    spriteAnimate();
    moveCamera(camera);

    if (m_dodgingLeft || m_dodgingRight)
    {
        if (m_dodgingLeft)
        {
            m_angle -= 20;
        }
        else if (m_dodgingRight)
        {
            m_angle += 20;
        }

        ++m_dodgeStepCount;

        if (m_dodgeStepCount > static_cast<int>(0.3 / Constants::updateStep))
        {
            m_dodgingLeft = false;
            m_dodgingRight = false;
            m_dodgeStepCount = 0;
            m_angle = 0.0;
            m_dodgeCooling = true;
        }
    }

    if (m_dodgeCooling)
    {
        ++m_dodgeStepCount;
        if (m_dodgeStepCount > static_cast<int>(m_dodgeCooldown / Constants::updateStep))
        {
            m_dodgeStepCount = 0;
            m_dodgeCooling = false;
        }
    }
}

//adjusts velocity of player depending on state of motion
void Player::motion()
{
    if (m_movement == AIRBORNE && !(m_velocity.gety() > m_yMaxSpeed))
    {
        //grounded characters fall when airborne
        m_velocity.add(Vector2D<double>{0, Constants::g});
    }
    //velocity increased/decreased unless at max horizontal velocity
    else if (m_movement == LEFT)
    {
        if (m_velocity.getx() - m_walkAcceleration >= -m_xMaxSpeed)
        {
            m_velocity.add(Vector2D<double>{-m_walkAcceleration, 0});
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(Vector2D<double>{-m_xMaxSpeed, 0});
        }
    }
    else if (m_movement == RIGHT)
    {
        if (m_velocity.getx() + m_walkAcceleration <= m_xMaxSpeed)
        {
            m_velocity.add(Vector2D<double>{m_walkAcceleration, 0});
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(Vector2D<double>{m_xMaxSpeed, 0});
        }
    }
    else
    {
        //deceleration when stopped moving
        m_velocity.xScale(0.83);
        if (std::abs(m_velocity.getx()) < 0.1)
        {
            m_velocity.xScale(0);
        }
    }

    if (m_velocity.getx() < -m_xMaxSpeed)
    {
        m_velocity.subtract(Vector2D<double>{ -0.1, 0 });
    }
    if (m_velocity.getx() > m_xMaxSpeed)
    {
        m_velocity.subtract(Vector2D<double>{ 0.1, 0 });
    }
}

void Player::cycleWalkAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 2)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 26)
        {
            m_spriteIndex = 5;
        }
    }
}

void Player::cycleIdleAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 6)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 4)
        {
            m_spriteIndex = 0;
        }
    }
}

void Player::spriteAnimate()
{
    if (m_movement == AIRBORNE)
    {
        m_spriteIndex = 27;
    }

    else if (m_movement == LEFT)
    {
        if (m_isFlipped == false)
        {
            m_isFlipped = true;
            m_spriteIndex = 5;
        }
        
        cycleWalkAnimation();
    }

    else if (m_movement == RIGHT)
    {
        if (m_isFlipped == true)
        {
            m_isFlipped = false;
            m_spriteIndex = 5;
        }

        cycleWalkAnimation();
    }

    else
    {
        cycleIdleAnimation();
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
        m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, flip);
    }
}

void Player::moveCamera(Camera& camera)
{
    camera.setPos(static_cast<int>(m_position.getx()) - (g_screenWidth / 2),
        static_cast<int>(m_position.gety()) - (g_screenHeight / 2));

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

void Player::dodgeLeft() 
{
    m_dodgingLeft = true;
    m_dodgeCooling = true;
}

void Player::dodgeRight() 
{
    m_dodgingRight = true;
    m_dodgeCooling = true;
}