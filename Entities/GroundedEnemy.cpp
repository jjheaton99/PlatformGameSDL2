#include "GroundedEnemy.h"
#include "Player.h"

GroundedEnemy::GroundedEnemy(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int hitPoints)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 50, 50, hitPoints)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 9.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 50;
    m_dstRect.h = 50;
}

void GroundedEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player)
{ 
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera for performance
    if (m_position.getx() > 1.0*camera.getx() - 1000.0 && m_position.getx() < 1.0*camera.getx() + 1.0*camera.getw() + 1000.0
        && m_position.gety() > 1.0*camera.gety() - 1000.0 && m_position.gety() < 1.0*camera.gety() + 1.0*camera.geth() + 1000.0)
    {
        motion();
        enemyControls(player);

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

        attackPlayer(player);
    }
}

void GroundedEnemy::enemyControls(Player& player)
{
    if (m_position.getx() < player.getPos().getx() + 20 - m_collider.getHitBox().w)
    {
        m_movement = RIGHT;
    }
    else if (m_position.getx() > player.getPos().getx() + 80)
    {
        m_movement = LEFT;
    }
    else
    {
        m_movement = STOP;
    }
}

//similar motion to player character
void GroundedEnemy::motion()
{
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
        break;

    case GroundedCharacter::LEFT:
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
        break;

    case GroundedCharacter::CLIMB_UP:
        break;

    case GroundedCharacter::CLIMB_DOWN:
        break;

    case GroundedCharacter::STOP:
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

void GroundedEnemy::attackPlayer(Player& player)
{
    if (m_collider.collideCheck(player.getCollider()) && !player.isInvincible())
    {
        player.removeHP(m_damage);
        player.startiFrames();
        m_velocity.scale(0);
        if (m_position.getx() < player.getPos().getx() + 50)
        {
            player.setVel(15.0, -5.0);
        }
        else if (m_position.getx() > player.getPos().getx() + 50)
        {
            player.setVel(-15.0, -5.0);
        }
    }
}