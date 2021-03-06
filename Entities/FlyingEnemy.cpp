#include "FlyingEnemy.h"
#include "Player.h"

FlyingEnemy::FlyingEnemy(std::string fileName, double xStartPos, double yStartPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : FlyingCharacter(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount), m_damage{ damage }
{}

void FlyingEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player)
{
    if (!m_killed)
    {
        if (m_hitPoints <= 0)
        {
            kill();
        }

        //only update when within certain distance from camera for performance
        if (m_position.getx() > 1.0 * camera.getx() - m_updateRange && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + m_updateRange
            && m_position.gety() > 1.0 * camera.gety() - m_updateRange && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + m_updateRange)
        {
            if (!m_immobilised)
            {
                enemyControls(player);
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
                collided = sweepMapCollideCheck(map) || attackPlayer(player);
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

            animateSprite();
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

bool FlyingEnemy::attackPlayer(std::shared_ptr<Character> player)
{
    if (!dynamic_cast<Player&>(*player).isInvincible() && (m_velocity.magnitude() < (m_position - player->getPos()).magnitude()))
    {
        Collider::sweptObstacleTuple sweptCollider{ player->getCollider(), Collider::xOverlap(m_collider, player->getCollider()), Collider::yOverlap(m_collider, player->getCollider()) };
        if (m_collider.sweptAABBDeflect(1.5, sweptCollider, m_position, m_velocity, player->getVel()))
        {
            dynamic_cast<Player&>(*player).removeHP(m_damage);
            return true;
        }
        else if (m_collider.collideCheck(player->getCollider()))
        {
            dynamic_cast<Player&>(*player).removeHP(m_damage);
            m_velocity.scale(-1.5);
            return true;
        }
    }
    return false;
}