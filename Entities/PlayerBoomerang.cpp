#include "PlayerBoomerang.h"

PlayerBoomerang::PlayerBoomerang(double xPos, double yPos, double xVel, double yVel, double colliderWidth, double colliderHeight, const char* fileName, int damage)
    : Projectile(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight), m_damage{damage}
{
    m_srcRect = { 0, 0, 32, 32 };

    m_dstRect.w = 60;
    m_dstRect.h = 60;
}

void PlayerBoomerang::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player)
{
    if (m_flying)
    {
        motion();
        m_distanceTravelled += m_velocity.magnitude();

        m_angle += 20.0;

        //put enemy collide check before map to ensure it hits
        if (enemyCollideCheck(enemies))
        {
            m_returningToPlayer = true;
            m_target = player;
        }

        //passes through map if returning to player
        if (!m_returningToPlayer)
        {
            if (sweepMapCollideCheck(map))
            {
                m_returningToPlayer = true;
                m_target = player;
            }
        }

        //limits distance boomerang can travel
        if (m_distanceTravelled > m_range && !m_returningToPlayer)
        {
            m_returningToPlayer = true;
            m_target = player;
        }

        //when returning needs to collide check with player
        if (m_target.lock() == player)
        {
            Collider::sweptObstacleTuple sweptCollider{ m_target.lock()->getCollider(), Collider::xOverlap(m_collider, m_target.lock()->getCollider()),
                Collider::yOverlap(m_collider, m_target.lock()->getCollider()) };
            if (m_collider.sweptAABBCheck(m_velocity, m_target.lock()->getVel(), sweptCollider).first != Collider::NONE)
            {
                m_flying = false;
                m_returningToPlayer = false;
                m_target.reset();
            }
        }

        m_position.add(m_velocity);
        setCollider();

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());
    }
    else
    {
        setPos(player->getPos().getx() + 20.0, player->getPos().gety() + 20.0);
    }
}

bool PlayerBoomerang::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    int projectileColumn{ static_cast<int>(m_collider.getHitBox().x / Constants::tileSize) };
    int projectileRow{ static_cast<int>(m_collider.getHitBox().y / Constants::tileSize) };

    getCollideTiles(map, projectileRow, projectileColumn);

    //sort by greatest overlaps
    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const auto& a, const auto& b) {
        return ((std::get<1>(a) > std::get<1>(b) && std::get<1>(a) > std::get<2>(b)) || (std::get<2>(a) > std::get<1>(b) && std::get<2>(a) > std::get<2>(b)));
        }
    );

    for (auto& sweptCollider : m_solidColliders)
    {
        if (m_collider.sweptAABBDeflect(1.0, sweptCollider, m_position, m_velocity, Vector2D<double>{0.0, 0.0}))
        {
            setCollider();
            return true;
        }
    }
    return false;
}

bool PlayerBoomerang::aquireTargetEnemy(const std::vector<std::shared_ptr<Character>>& enemies)
{
    if (!m_target.lock())
    {
        double enemyDistance;
        double closestEnemyDistance;
        closestEnemyDistance = std::numeric_limits<double>::max();
        int closestEnemyIndex{ -1 };

        for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
        {
            if (enemies[i])
            {
                enemyDistance = (enemies[i]->getPos() - m_position).magnitude();
                if (enemyDistance < closestEnemyDistance)
                {
                    closestEnemyIndex = i;
                    closestEnemyDistance = enemyDistance;
                }
            }
        }

        if (closestEnemyIndex >= 0)
        {
            m_target = enemies[closestEnemyIndex];
            return true;
        }
        else
        {
            m_target.reset();
            return false;
        }
    }
    return false;
}

bool PlayerBoomerang::enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies)
{
    if (!m_returningToPlayer)
    {
        aquireTargetEnemy(enemies);

        if (m_target.lock())
        {
            Collider::sweptObstacleTuple sweptCollider{ m_target.lock()->getCollider(), Collider::xOverlap(m_collider, m_target.lock()->getCollider()),
                Collider::yOverlap(m_collider, m_target.lock()->getCollider()) };
            if (m_collider.sweptAABBCheck(m_velocity, m_target.lock()->getVel(), sweptCollider).first != Collider::NONE)
            {
                m_target.lock()->removeHP(m_damage);
                m_target.lock()->addVel(m_velocity);
                return true;
            }
        }
    }
    return false;
}

void PlayerBoomerang::motion()
{
    double scaleFactor{};
    double velMag{};
    if (m_target.lock())
    {
        Vector2D<double> relPos{ m_target.lock()->getPos() + Vector2D<double>{m_target.lock()->getCollider().getHitBox().w / 2.0 - m_dstRect.w / 2.0, 
            m_target.lock()->getCollider().getHitBox().h / 2.0 - m_dstRect.h / 2.0} - m_position };
        scaleFactor = m_acceleration / relPos.magnitude();
        m_velocity.add(scaleFactor * relPos);
        velMag = m_velocity.magnitude();

        if (velMag > m_maxSpeed)
        {
            scaleFactor = m_acceleration / velMag;
            m_velocity.subtract(scaleFactor * m_velocity);
        }
    }

    else
    {
        velMag = m_velocity.magnitude();
        scaleFactor = m_acceleration / velMag;
        m_velocity.add(scaleFactor * m_velocity);
        velMag = m_velocity.magnitude();

        if (velMag > m_maxSpeed)
        {
            scaleFactor = m_acceleration / velMag;
            m_velocity.subtract(scaleFactor * m_velocity);
        }
    }
}

void PlayerBoomerang::cameraDraw(const Camera& camera) const
{
    if (m_flying)
    {
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, SDL_FLIP_NONE);
        }
    }
}

void PlayerBoomerang::throwLeft(const std::vector<std::shared_ptr<Character>>& enemies)
{
    m_distanceTravelled = 0.0;
    m_flying = true;
    if (aquireTargetEnemy(enemies))
    {
        Vector2D<double> relPos{ m_target.lock()->getPos() + Vector2D<double>{m_target.lock()->getCollider().getHitBox().w / 2.0, m_target.lock()->getCollider().getHitBox().h / 2.0} - m_position };
        double scaleFactor = m_maxSpeed / relPos.magnitude();
        m_velocity = scaleFactor * relPos;
    }
    else
    {
        m_velocity = Vector2D<double>{ -m_maxSpeed, 0.0 };
    }
}

void PlayerBoomerang::throwRight(const std::vector<std::shared_ptr<Character>>& enemies)
{
    m_distanceTravelled = 0.0;
    m_flying = true;
    if (aquireTargetEnemy(enemies))
    {
        Vector2D<double> relPos{ m_target.lock()->getPos() + Vector2D<double>{m_target.lock()->getCollider().getHitBox().w / 2.0, m_target.lock()->getCollider().getHitBox().h / 2.0} - m_position };
        double scaleFactor = m_maxSpeed / relPos.magnitude();
        m_velocity = scaleFactor * relPos;
    }
    else
    {
        m_velocity = Vector2D<double>{ m_maxSpeed, 0.0 };
    }
}