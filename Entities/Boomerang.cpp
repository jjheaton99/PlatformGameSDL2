#include "Boomerang.h"

Boomerang::Boomerang(double xPos, double yPos, double xVel, double yVel, double colliderWidth, double colliderHeight, std::string fileName, int damage)
    : Projectile(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight, damage)
{
    m_srcRect = { 0, 0, 32, 32 };

    m_dstRect.w = 60;
    m_dstRect.h = 60;
}

void Boomerang::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player)
{
    if (m_throwLeft)
    {
        m_flying = true;
        m_throwLeft = false;
        aquireTargetEnemy(enemies);
        m_velocity = Vector2D<double>{ -m_maxSpeed, 0.0 };
    }
    else if (m_throwRight)
    {
        m_flying = true;
        m_throwRight = false;
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
        if (m_isCooling)
        {
            ++m_coolDownCount;
            if (m_coolDownCount > static_cast<int>(m_coolDown / Constants::updateStep))
            {
                m_isCooling = false;
                m_coolDownCount = 0;
            }
        }
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
            if (enemies[i] && !enemies[i]->isDying() && m_prevTarget.lock() != enemies[i])
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

void Boomerang::cameraDraw(const Camera& camera) const
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

void Boomerang::throwLeft()
{
    if (!m_isCooling && !m_flying)
    {
        m_throwLeft = true;
    }
}

void Boomerang::throwRight()
{
    if (!m_isCooling && !m_flying)
    {
        m_throwRight = true;
    }
}

double Boomerang::getCooldownFraction() const
{
    if (m_flying)
    {
        return 1.0;
    }
    else if (!m_isCooling)
    {
        return 0.0;
    }
    else
    {
        return 1.0 - (static_cast<double>(m_coolDownCount) / (m_coolDown / Constants::updateStep));
    }
}