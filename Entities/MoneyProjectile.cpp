#include "MoneyProjectile.h"

MoneyProjectile::MoneyProjectile(std::shared_ptr<Player> player, double xPos, double yPos, double xVel, double yVel, double colliderWidth, double colliderHeight, std::string fileName)
    : Projectile(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight), m_player{ player }
{
    m_srcRect = { 0, 0, 3, 3 };

    m_value = MTRandom::getRandomInt(5, 20);

    m_dstRect.w = m_value;
    m_dstRect.h = m_value;

    m_homingDelay = MTRandom::getRandomInt(0, 60);
}

void MoneyProjectile::update(const std::vector<std::vector<Tile>>& map, const Camera& camera)
{
    motion();

    //when returning needs to collide check with player
    bool hit{ false };
    if (m_collider.collideCheck(m_player->getCollider()))
    {
        hit = true;
    }
    else
    {
        Collider::sweptObstacleTuple sweptCollider{ m_player->getCollider(), Collider::xOverlap(m_collider, m_player->getCollider()),
            Collider::yOverlap(m_collider, m_player->getCollider()) };
        if (m_collider.sweptAABBCheck(m_velocity, m_player->getVel(), sweptCollider).first != Collider::NONE)
        {
            hit = true;
        }
    }

    if (hit)
    {
        m_player->addMoney(m_value);
        m_terminated = true;
    }

    m_position.add(m_velocity);
    setCollider();

    m_dstRect.x = static_cast<int>(m_position.getx());
    m_dstRect.y = static_cast<int>(m_position.gety());
}

void MoneyProjectile::motion()
{
    if (++m_homingDelayCount > m_homingDelay)
    {
        double scaleFactor{};
        double velMag{};

        Vector2D<double> relPos{ Vector2D<double>{m_player->getCollider().getHitBox().x + 50.0,
            m_player->getCollider().getHitBox().y + 50.0} - m_position };
        scaleFactor = m_acceleration / relPos.magnitude();
        m_velocity.add(scaleFactor * relPos);
        velMag = m_velocity.magnitude();

        if (velMag > m_maxSpeed)
        {
            //subtract excess speed
            scaleFactor = (velMag - m_maxSpeed) / velMag;
            m_velocity.subtract(scaleFactor * m_velocity);
        }
    }
    else
    {
        m_velocity.scale(0.98);
    }
}