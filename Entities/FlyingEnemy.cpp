#include "FlyingEnemy.h"
#include "Player.h"

FlyingEnemy::FlyingEnemy(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight, int hitPoints, int damage)
    : FlyingCharacter(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints), m_damage{ damage }
{}

void FlyingEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player)
{
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera for performance
    if (m_position.getx() > 1.0 * camera.getx() - 1000.0 && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + 1000.0
        && m_position.gety() > 1.0 * camera.gety() - 1000.0 && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + 1000.0)
    {
        enemyControls(player);

        //edge check goes before map collision check to prevent vector subcript error when going off the edge
        if (edgeCheck(camera))
        {
            //collider position is moved after each function that can change character position
            setCollider();
        }

        bool collided{ sweepMapCollideCheck(map) || attackPlayer(player) };
        if (collided)
        {
            setCollider();
        }

        if (!collided)
        {
            m_position.add(m_velocity);
            setCollider();
        }

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());
    }
}

bool FlyingEnemy::attackPlayer(Character& player)
{
    if (!dynamic_cast<Player&>(player).isInvincible() && (m_velocity.magnitude() < (m_position - player.getPos()).magnitude()))
    {
        Collider::sweptObstacleTuple sweptCollider{ player.getCollider(), Collider::xOverlap(m_collider, player.getCollider()), Collider::yOverlap(m_collider, player.getCollider()) };
        if (m_collider.sweptAABBDeflect(1.5, sweptCollider, m_position, m_velocity, player.getVel()))
        {
            player.removeHP(m_damage);
            dynamic_cast<Player&>(player).startiFrames();
            return true;
        }
        else if (m_collider.collideCheck(player.getCollider()))
        {
            player.removeHP(m_damage);
            dynamic_cast<Player&>(player).startiFrames();
            m_velocity.scale(-1.5);
            return true;
        }
    }
    return false;
}
