#include "Enemy.h"

Enemy::Enemy(int damage)
    : m_damage{damage}
{}

void Enemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player)
{
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera for performance
    if (m_position.getx() > 1.0 * camera.getx() - 1000.0 && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + 1000.0
        && m_position.gety() > 1.0 * camera.gety() - 1000.0 && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + 1000.0)
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