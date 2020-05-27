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

void FlyingEnemy::attackPlayer(Character& player)
{
    if (m_collider.collideCheck(dynamic_cast<Player&>(player).getCollider()) && !dynamic_cast<Player&>(player).isInvincible())
    {
        player.removeHP(m_damage);
        dynamic_cast<Player&>(player).startiFrames();
        //m_velocity.scale(-1.0);
        //setVel((-5.0 / m_velocity.magnitude()) * m_velocity);
    }
}
