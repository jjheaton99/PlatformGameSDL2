#include "EnemyProjectile.h"

EnemyProjectile::EnemyProjectile(std::shared_ptr<Character> enemy, Vector2D<double> relStartPos, std::shared_ptr<Character> player, 
    double xVel, double yVel, int colliderWidth, int colliderHeight, int damage, const char* fileName)
    : Projectile(fileName, enemy->getPos().getx() + relStartPos.getx(), enemy->getPos().gety() + relStartPos.gety(), xVel, yVel, colliderWidth, colliderHeight, damage)
{
    m_player = player;
}

void EnemyProjectile::update(const std::vector<std::vector<Tile>>& map, const Camera& camera)
{
    motion();

    if (sweepMapCollideCheck(map))
    {
        m_terminated = true;
    }

    else if (playerCollideCheck())
    {
        m_terminated = true;
        m_player.lock()->removeHP(m_damage);
    }

    m_position.add(m_velocity);
    setCollider();

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

bool EnemyProjectile::playerCollideCheck()
{
    if (m_collider.collideCheck(m_player.lock()->getCollider()))
    {
        return true;
    }
    else
    {
        double xOverlap{ Collider::xOverlap(m_collider, m_player.lock()->getCollider()) };
        double yOverlap{ Collider::yOverlap(m_collider, m_player.lock()->getCollider()) };
        Collider::sweptObstacleTuple sweptCollider{ m_player.lock()->getCollider(), xOverlap, yOverlap };
        if (m_collider.sweptAABBCheck(m_velocity, m_player.lock()->getVel(), sweptCollider).first != Collider::NONE)
        {
            return true;
        }
    }
    return false;
}