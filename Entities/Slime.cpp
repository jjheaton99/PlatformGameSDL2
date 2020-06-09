#include "Slime.h"
#include "Player.h"

Slime::Slime(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int colliderWidth, int colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : GroundedEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount, damage)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 20 * i, 0, 20, 15 });
    }

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 6.0;
    m_walkAcceleration = 0.8;
    m_stopDeceleration = 0.9;

    m_dstRect.w = 100;
    m_dstRect.h = 75;
}

void Slime::enemyControls(std::shared_ptr<Character> player)
{
    if (m_movement != AIRBORNE)
    {
        if (m_position.getx() < player->getPos().getx() + 30 - m_collider.getHitBox().w)
        {
            m_movement = RIGHT;
            m_facingLeft = false;
            if (m_landed)
            {
                m_movement = STOP;
            }
        }
        else if (m_position.getx() > player->getPos().getx() + 70)
        {
            m_movement = LEFT;
            m_facingLeft = true;
            if (m_landed)
            {
                m_movement = STOP;
            }
        }
        else
        {
            m_movement = STOP;
        }

        if (!m_landed && MTRandom::getRandomInt(0, 99) < 1)
        {
            m_movement = AIRBORNE;
            //jumps with randomised velocity boost
            addVel(Vector2D<double>{(m_velocity.getx() > 0.0 ? 1.0 : -1.0) * MTRandom::getRandomDouble(0.0, 10.0), -MTRandom::getRandomDouble(20.0, 30.0)});
            m_jumped = true;
            m_spriteIndex = 8;
        }
    }
}

void Slime::cycleWalkAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 4)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 7)
        {
            m_spriteIndex = 0;
        }
    }
}

void Slime::cycleJumpAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 4)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 11)
        {
            m_jumped = false;
            m_spriteIndex = 12;
        }
    }
}

void Slime::cycleLandAnimation()
{
    if (m_spriteIndex < 13)
    {
        m_spriteIndex = 13;
    }

    ++m_animationStep;
    if (m_animationStep >= 4)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex == 18)
        {
            m_landed = false;
        }
    }
}

void Slime::animateSprite()
{
    if (m_jumped)
    {
        cycleJumpAnimation();
    }
    else if (m_landed)
    {
        cycleLandAnimation();
    }
    else
    {
        switch (m_movement)
        {
        case GroundedCharacter::AIRBORNE:
            m_spriteIndex = 12;
            break;
        case GroundedCharacter::LEFT:
        case GroundedCharacter::RIGHT:
        case GroundedCharacter::STOP:
            cycleWalkAnimation();
            break;
        default:
            break;
        }
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}