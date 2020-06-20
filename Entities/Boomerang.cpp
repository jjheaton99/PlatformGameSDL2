#include "Boomerang.h"

Boomerang::Boomerang()
    : PlayerRangedAttack("Assets/Attacks/boomerang.png", 0.0, 0.0, 0.0, 0.0, 10.0, 10.0, 200, 3.0)
{
    m_itemType = ItemType::BOOMERANG;

    m_srcRect = { 0, 0, 32, 32 };

    m_dstRect.w = 60;
    m_dstRect.h = 60;
}

void Boomerang::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player)
{
    if (m_shootLeft)
    {
        m_flying = true;
        m_shootLeft = false;
        aquireTargetEnemy(enemies);
        m_velocity = Vector2D<double>{ -m_maxSpeed, 0.0 };
    }
    else if (m_shootRight)
    {
        m_flying = true;
        m_shootRight = false;
        aquireTargetEnemy(enemies);
        m_velocity = Vector2D<double>{ m_maxSpeed, 0.0 };
    }

    if (m_flying)
    {
        motion();
        m_distanceTravelled += m_velocity.magnitude();

        //for rotating animation/sound
        if (++m_flyingSoundCount > 5)
        {
            m_flyingSound.play();
            m_flyingSoundCount = 0;
        }
        m_angle += 20.0;

        //put enemy collide check before map to ensure it hits
        if (enemyCollideCheck(enemies))
        {
            ++m_collisionCount;
            if (m_collisionCount >= m_maxCollisions)
            {
                returnToPlayer(player);
            }
        }

        //passes through map if returning to player
        if (!m_returningToPlayer)
        {
            if (sweepMapCollideCheck(map))
            {
                ++m_collisionCount;
                //allow boomerang to find new target
                m_prevTarget = m_target;
                m_target.reset();
                if (m_collisionCount >= m_maxCollisions)
                {
                    returnToPlayer(player);
                }
            }
        }

        //limits distance boomerang can travel
        if (m_distanceTravelled > m_range && !m_returningToPlayer)
        {
            returnToPlayer(player);
        }

        //when returning needs to collide check with player
        if (m_target.lock() == player)
        {
            bool hit{ false };
            if (m_collider.collideCheck(m_target.lock()->getCollider()))
            {
                hit = true;
            }
            else
            {
                Collider::sweptObstacleTuple sweptCollider{ m_target.lock()->getCollider(), Collider::xOverlap(m_collider, m_target.lock()->getCollider()),
                    Collider::yOverlap(m_collider, m_target.lock()->getCollider()) };
                if (m_collider.sweptAABBCheck(m_velocity, m_target.lock()->getVel(), sweptCollider).first != Collider::NONE)
                {
                    hit = true;
                }
            }

            if (hit)
            {
                m_flying = false;
                m_returningToPlayer = false;
                m_isCooling = true;
                m_target.reset();
                setPos(player->getPos().getx() + 20.0, player->getPos().gety() + 20.0);
                m_distanceTravelled = 0.0;
                m_collisionCount = 0;
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
        coolDown();
    }
}

void Boomerang::returnToPlayer(std::shared_ptr<Character> player)
{
    m_returningToPlayer = true;
    m_target = player;
    m_prevTarget.reset();
}

bool Boomerang::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
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
            m_prevTarget.reset();
            m_hitWallSound.play();
            return true;
        }
    }
    return false;
}

//searches for enemy closest to boomerang and locks on
bool Boomerang::aquireTargetEnemy(const std::vector<std::shared_ptr<Character>>& enemies)
{
    if (!m_target.lock())
    {
        double enemyDistance;
        double closestEnemyDistance;
        closestEnemyDistance = std::numeric_limits<double>::max();
        int closestEnemyIndex{ -1 };

        for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
        {
            if (enemies[i] && enemies[i]->isInUpdateRange() && !enemies[i]->isDying() && m_prevTarget.lock() != enemies[i])
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

bool Boomerang::enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies)
{
    bool hit{ false };
    if (!m_returningToPlayer)
    {
        aquireTargetEnemy(enemies);

        if (m_target.lock())
        {
            if (m_collider.collideCheck(m_target.lock()->getCollider()))
            {
                hit = true;
            }
            else
            {
                Collider::sweptObstacleTuple sweptCollider{ m_target.lock()->getCollider(), Collider::xOverlap(m_collider, m_target.lock()->getCollider()),
                    Collider::yOverlap(m_collider, m_target.lock()->getCollider()) };
                if (m_collider.sweptAABBCheck(m_velocity, m_target.lock()->getVel(), sweptCollider).first != Collider::NONE)
                {
                    hit = true;
                }
            }

            if (hit)
            {
                m_target.lock()->removeHP(m_damage);
                m_target.lock()->addVel(0.5 * m_velocity);
                m_prevTarget = m_target;
                m_target.reset();
            }
        }
    }
    return hit;
}

void Boomerang::motion()
{
    double scaleFactor{};
    double velMag{};
    if (m_target.lock())
    {
        Vector2D<double> relPos{ Vector2D<double>{m_target.lock()->getCollider().getHitBox().x, 
            m_target.lock()->getCollider().getHitBox().y} - m_position };
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
        velMag = m_velocity.magnitude();
        scaleFactor = m_acceleration / velMag;
        m_velocity.add(scaleFactor * m_velocity);
        velMag = m_velocity.magnitude();

        if (velMag > m_maxSpeed)
        {
            scaleFactor = (velMag - m_maxSpeed) / velMag;
            m_velocity.subtract(scaleFactor * m_velocity);
        }
    }
}