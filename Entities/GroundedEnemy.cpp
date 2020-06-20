#include "GroundedEnemy.h"
#include "Player.h"

GroundedEnemy::GroundedEnemy(std::string fileName, double xStartPos, double yStartPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount), m_damage{ damage }
{}

void GroundedEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player)
{ 
    if (!m_killed)
    {
        if (m_hitPoints <= 0)
        {
            kill();
        }

        //only update when within certain distance from camera
        if (m_position.getx() > 1.0 * camera.getx() - m_updateRange && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + m_updateRange
            && m_position.gety() > 1.0 * camera.gety() - m_updateRange && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + m_updateRange)
        {
            if (!m_immobilised)
            {
                enemyControls(player);
            }
            else
            {
                m_movement = AIRBORNE;
            }

            //edge check goes before map collision check to prevent vector subcript error when going off the edge
            if (edgeCheck(camera))
            {
                //collider position is moved after each function that can change character position
                setCollider();
            }

            bool collided{ false };
            if (!m_immobilised)
            {
                bool airborne{ m_movement == AIRBORNE };
                bool collided{ sweepMapCollideCheck(map) || attackPlayer(player) };
                bool landed{ m_movement != AIRBORNE };
                //if enemy just landed from jumping
                if (airborne && landed)
                {
                    m_landed = true;
                }
            }
            else
            {
                collided = immobilisedSweepMapCollideCheck(map) || attackPlayer(player);
            }

            if (collided)
            {
                setCollider();
            }
            else
            {
                m_position.add(m_velocity);
                setCollider();
            }

            m_dstRect.x = static_cast<int>(m_position.getx());
            m_dstRect.y = static_cast<int>(m_position.gety());

            if (!m_immobilised)
            {
                motion();
            }
            animateSprite();
            attackPlayer(player);
            cycleDamageFlash();

            m_inUpdateRange = true;
        }
        else
        {
            m_inUpdateRange = false;
        }
    }
    else
    {
        killAnimation();
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
        m_velocity.xScale(m_stopDeceleration);
        if (std::abs(m_velocity.getx()) < 0.0001)
        {
            m_velocity.xScale(0);
        }
        break;

    default:
        break;
    }
}

bool GroundedEnemy::attackPlayer(std::shared_ptr<Character> player)
{
    if (!dynamic_cast<Player&>(*player).isInvincible() && (m_velocity.magnitude() < (m_position - player->getPos()).magnitude()))
    {
        Collider::sweptObstacleTuple sweptCollider{ player->getCollider(), Collider::xOverlap(m_collider, player->getCollider()), Collider::yOverlap(m_collider, player->getCollider()) };
        if (m_collider.sweptAABBDeflect(1.0, sweptCollider, m_position, m_velocity, player->getVel()))
        {
            dynamic_cast<Player&>(*player).removeHP(m_damage);
            return true;
        }
        else if (m_collider.collideCheck(player->getCollider()))
        {
            dynamic_cast<Player&>(*player).removeHP(m_damage);
            m_velocity.scale(-1.0);
            return true;
        }
    }
    return false;
}