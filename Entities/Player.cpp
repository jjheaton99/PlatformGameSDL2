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

    m_yMaxSpeed = 100.0;
    m_xMaxSpeed = 12.0;
    m_walkAcceleration = 0.8;
    m_climbSpeed = 12.0;
    
    m_spriteIndex = 0;

    m_dstRect.w = 100;
    m_dstRect.h = 100;
}

void Player::update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<Character*>& enemies)
{
    motion();

    //edge check goes before map collision check to prevent vector subcript error when going off the edge
    if (edgeCheck(camera))
    {
        //collider position is moved after each function that can change character position
        setCollider();
    }

    bool collided{ sweepMapCollideCheck(map) };
    if (collided)
    { 
        setCollider(); 
    }

    //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
    //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';
    //std::cout << m_collider.getHitBox().y << "   " << m_collider.getHitBox().x << '\n';

    if (!collided)
    {
        m_position.add(m_velocity);
        setCollider();
    }

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());

    spriteAnimate();
    moveCamera(camera);

    if (m_dodgingLeft || m_dodgingRight)
    {
        if (m_dodgingLeft)
        {
            m_angle -= (360.0 / m_dodgeFrames);
        }
        else if (m_dodgingRight)
        {
            m_angle += (360.0 / m_dodgeFrames);
        }

        ++m_dodgeStepCount;

        if (m_dodgeStepCount > m_dodgeFrames)
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

    if (m_hasCrouched)
    {
        ++m_crouchStepCount;
        if (m_crouchStepCount > static_cast<int>(std::sqrt((2 * Constants::tileSize) / Constants::g)))
        {
            m_hasCrouched = false;
            m_crouchStepCount = 0;
        }
    }

    //attack texture position set with an offset from player position 
    m_sideAttack.setPos(1.0 * m_position.getx() + 50.0, 1.0 * m_position.gety() + 65.0);
    m_sideAttack.update(enemies);
}

//adjusts velocity of player depending on state of motion
void Player::motion()
{
    if (m_crouched)
    {
        m_movement = AIRBORNE;
        m_velocity.xScale(0.8);
        {
            if (std::abs(m_velocity.getx()) < 0.0001)
            {
                m_velocity.xScale(0);
            }
        }
    }

    switch (m_movement)
    {
    case GroundedCharacter::AIRBORNE:
        if (m_velocity.gety() + Constants::g <= m_yMaxSpeed)
        {
            m_velocity.add(0, Constants::g);
        }
        else
        {
            m_velocity.yScale(0.0);
            m_velocity.add(0, m_yMaxSpeed);
        }

        if (m_velocity.gety() == m_yMaxSpeed || std::abs(m_velocity.getx()) > m_xMaxSpeed)
        {
            m_velocity.xScale(0.93);
            if (std::abs(m_velocity.getx()) < 0.0001)
            {
                m_velocity.xScale(0);
            }
        }
        break;

    case GroundedCharacter::LEFT:
        //velocity increased/decreased unless at max horizontal velocity
        if (m_velocity.getx() - m_walkAcceleration >= -m_xMaxSpeed)
        {
            m_velocity.add(-m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() < -m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(-m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::RIGHT:
        if (m_velocity.getx() + m_walkAcceleration <= m_xMaxSpeed)
        {
            m_velocity.add(m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() > m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::CLIMB_STOP:
        m_velocity.scale(0);
        break;

    case GroundedCharacter::CLIMB_UP:
        setVel(0, -m_climbSpeed);
        break;

    case GroundedCharacter::CLIMB_DOWN:
        setVel(0, m_climbSpeed);
        break;

    case GroundedCharacter::STOP:
        //deceleration when stopped moving
        m_velocity.xScale(0.7);
        if (std::abs(m_velocity.getx()) < 0.0001)
        {
            m_velocity.xScale(0);
        }
        break;

    default:
        break;
    }
}

void Player::cycleWalkAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 2)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 21)
        {
            m_spriteIndex = 0;
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
    switch (m_movement)
    {
    case GroundedCharacter::AIRBORNE:
        m_spriteIndex = 22;
        break;

    case GroundedCharacter::LEFT:
        if (m_facingLeft == false)
        {
            m_facingLeft = true;
            m_spriteIndex = 0;
        }
        cycleWalkAnimation();
        break;

    case GroundedCharacter::RIGHT:
        if (m_facingLeft == true)
        {
            m_facingLeft = false;
            m_spriteIndex = 0;
        }
        cycleWalkAnimation();
        break;

    case GroundedCharacter::STOP:
        //cycleIdleAnimation();
        m_spriteIndex = 23;
        break;

    default:
        break;
    }

    if (m_sideAttack.isAttacking())
    {
        m_spriteIndex = 24;
    }
    else if (isDodging())
    {
        m_spriteIndex = 22;
    }
    else if (m_crouched)
    {
        m_spriteIndex = 23;
    }
    else if (isClimbing())
    {
        m_spriteIndex = 25;
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}

void Player::cameraDraw(const Camera& camera) const
{   
    SDL_RendererFlip flip{ SDL_FLIP_NONE };
    if (m_facingLeft)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, flip);
    }

    m_sideAttack.cameraDraw(camera);
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

void Player::attackLeft()
{
    m_facingLeft = true;
    m_sideAttack.faceLeft();
    m_sideAttack.attack();
    if (m_movement == AIRBORNE)
    {
        m_velocity.add(-5.0, -5.0);
    }
}

void Player::attackRight()
{
    m_facingLeft = false;
    m_sideAttack.faceRight();
    m_sideAttack.attack();
    if (m_movement == AIRBORNE)
    {
        m_velocity.add(5.0, -5.0);
    }
}

void Player::dodgeCancel()
{ 
    if (isDodging())
    {
        m_dodgeStepCount = m_dodgeFrames + 1;
    }
}

void Player::attackCancel()
{
    if (isAttacking())
    {
        m_sideAttack.cancel();
    }
}