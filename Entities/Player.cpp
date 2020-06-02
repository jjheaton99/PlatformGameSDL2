#include "Player.h"
#include "GroundedEnemy.h"

//Player collision hitBox width = 0.56 * collision hitBox height
Player::Player(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int hitPoints)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 56, 82, hitPoints)
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
    m_climbSpeed = 13.0;
    
    m_spriteIndex = 0;

    m_dstRect.w = 100;
    m_dstRect.h = 100;
}

Player::~Player()
{}

void Player::update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<std::shared_ptr<Character>>& enemies)
{
    //std::cout << m_hitPoints << '\n';
    if (m_hitPoints <= 0)
    {
        //kill();
    }

    motion();

    //edge check goes before map collision check to prevent vector subcript error when going off the edge
    if (edgeCheck(camera))
    {
        //collider position is moved after each function that can change character position
        setCollider();
    }

    bool collided{ sweepMapCollideCheck(map) };

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

    if (!isDodging() && m_invincible)
    {
        ++m_iFrameCount;
        if (m_iFrameCount > m_iFrames)
        {
            m_invincible = false;
            m_iFrameCount = 0;
        }
    }

    if (m_dodgingLeft || m_dodgingRight)
    {
        if (m_dodgingLeft)
        {
            m_angle -= (360.0 / m_dodgeFrames);
            if (m_movement != AIRBORNE && m_velocity.getx() > -m_minDodgeVel)
            {
                setVel(-m_minDodgeVel, m_velocity.gety());
            }
        }
        else if (m_dodgingRight)
        {
            m_angle += (360.0 / m_dodgeFrames);
            if (m_movement != AIRBORNE && m_velocity.getx() < m_minDodgeVel)
            {
                setVel(m_minDodgeVel, m_velocity.gety());
            }
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
            m_invincible = false;
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

    if (m_jumpingHigher)
    {
        ++m_jumpHigherCount;
        if (m_jumpHigherCount > 11)
        {
            m_jumpingHigher = false;
            m_jumpHigherCount = 0;
        }
    }

    //attack texture position set with an offset from player position 
    m_sideAttack.setPos(1.0 * m_position.getx() + 50.0, 1.0 * m_position.gety() + 65.0);
    m_sideAttack.update(enemies);

    if (m_throwBoomerang)
    {
        if (m_facingLeft && !m_boomerang.isFlying())
        {
            m_boomerang.throwLeft(enemies);
        }
        else if (!m_facingLeft && !m_boomerang.isFlying())
        {
            m_boomerang.throwRight(enemies);
        }
        m_throwBoomerang = false;
    }
    m_boomerang.update(map, camera, enemies, shared_from_this());
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

    double fallAccel;
    if (m_velocity.gety() > 0)
    {
        fallAccel = 1.5 * Constants::g;
    }
    else if (m_jumpingHigher)
    {
        fallAccel = 0.2 * Constants::g;
    }
    else
    {
        fallAccel = Constants::g;
    }

    switch (m_movement)
    {
    case GroundedCharacter::AIRBORNE:
        if (m_velocity.gety() + fallAccel <= m_yMaxSpeed)
        {
            m_velocity.add(0, fallAccel);
        }
        else
        {
            m_velocity.yScale(0.0);
            m_velocity.add(0, m_yMaxSpeed);
        }

        //player will move slightly left or right in the air when keys are held
        if (m_floatingLeft)
        {
            if (m_velocity.getx() - m_floatAccel >= -m_maxFloatSpeed)
            {
                m_velocity.add(-m_floatAccel, 0.0);
            }
        }
        else if (m_floatingRight)
        {
            if (m_velocity.getx() + m_floatAccel <= m_maxFloatSpeed)
            {
                m_velocity.add(m_floatAccel, 0.0);
            }
        }
        if (std::abs(m_velocity.getx()) > m_xMaxSpeed) 
        {
            m_velocity.xScale(0.95);
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

    m_floatingLeft = false;
    m_floatingRight = false;
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

    if (m_sideAttack.isAttacking() || m_boomerang.isFlying())
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
    m_boomerang.cameraDraw(camera);
}

void Player::moveCamera(Camera& camera)
{
    camera.setPos(static_cast<int>(m_position.getx()) + (m_dstRect.w / 2) - (g_screenWidth / 2),
        static_cast<int>(m_position.gety()) + (m_dstRect.h / 2) - (g_screenHeight / 2));

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
    if (!boomerangIsFlying())
    {
        m_facingLeft = true;
        m_sideAttack.faceLeft();
        m_sideAttack.attack();
        if (m_movement == AIRBORNE)
        {
            m_velocity.add(-5.0, -5.0);
        }
    }
}

void Player::attackRight()
{
    if (!boomerangIsFlying())
    {
        m_facingLeft = false;
        m_sideAttack.faceRight();
        m_sideAttack.attack();
        if (m_movement == AIRBORNE)
        {
            m_velocity.add(5.0, -5.0);
        }
    }
}

void Player::dodgeLeft()
{
    m_dodgingLeft = true;
    m_invincible = true;
    m_iFrameCount = m_iFrames + 1;
}

void Player::dodgeRight()
{ 
    m_dodgingRight = true;
    m_invincible = true;
    m_iFrameCount = m_iFrames + 1;
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

void Player::startiFrames()
{ 
    if (!m_invincible)
    {
        m_invincible = true;
    }
}

void Player::makeAirborne()
{
    m_movement = AIRBORNE;
    setCollider();
}

void Player::setCollider()
{
    if (m_movement == AIRBORNE)
    {
        m_collider.setDimensions(56, 100);
        m_collider.setPosition(m_position.getx() + 22.0, m_position.gety());
    }
    else
    {
        m_collider.setDimensions(56, 82);
        m_collider.setPosition(m_position.getx() + 22.0, m_position.gety() + 18.0);
    }
}

void Player::throwBoomerang() 
{ 
    if (!isAttacking())
    {
        m_throwBoomerang = true;
    }
}

double Player::getDodgeCooldownFraction() const
{
    if (isDodging())
    {
        return 1.0;
    }
    else if (!m_dodgeCooling)
    {
        return 0.0;
    }
    else
    {
        return 1.0 - (static_cast<double>(m_dodgeStepCount) / (m_dodgeCooldown / Constants::updateStep));
    }
}

void Player::floatLeft()
{ 
    m_floatingLeft = true;
    m_floatingRight = false;
    m_facingLeft = true;
}

void Player::floatRight() 
{
    m_floatingRight = true;
    m_floatingLeft = false;
    m_facingLeft = false;
}