#include "BlackHole.h"

BlackHole::BlackHole()
    : PlayerRangedAttack("Assets/Attacks/blackHole.png", 0.0, 0.0, 0.0, 0.0, 56.0, 56.0, 1, 0.5)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 50 * i, 0, 50, 50 });
    }

    m_itemType = ItemType::BLACK_HOLE;

    m_srcRect = { 0, 0, 50, 50 };

    m_dstRect.w = 156;
    m_dstRect.h = 156;

    m_texture.setBlendingMode(SDL_BLENDMODE_BLEND);
    m_texture.setAlpha(150);
}

void BlackHole::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player)
{
    if (m_shootLeft)
    {
        m_flying = true;
        m_shootLeft = false;
        m_velocity = Vector2D<double>{ -m_speed, 0.0 };
    }
    else if (m_shootRight)
    {
        m_flying = true;
        m_shootRight = false;
        m_velocity = Vector2D<double>{ m_speed, 0.0 };
    }

    if (m_flying)
    {
        if (m_flyingCount == 0)
        {
            m_dstRect.w = 156;
            m_dstRect.h = 156;
            m_flyingSound.play();
        }

        m_angle -= 6.0;
        if (++m_animationStep >= 4)
        {
            m_animationStep = 0;
            ++m_spriteIndex;
            if (m_spriteIndex > 6)
            {
                m_spriteIndex = 0;
            }
        }

        m_srcRect = m_spriteRects[m_spriteIndex];

        pullEnemies(enemies);
        pulledEnemiesCollideCheck();

        //passes through map if returning to player
        if (sweepMapCollideCheck(map))
        {
            m_collided = true;
        }

        m_position.add(m_velocity);
        setCollider();

        motion();

        if (m_implosionCount == 0)
        {
            m_dstRect.x = static_cast<int>(m_position.getx());
            m_dstRect.y = static_cast<int>(m_position.gety());
        }

        if (++m_flyingCount >= static_cast<int>(m_duration / Constants::updateStep))
        {
            m_collided = true;

            int newDstRectw{ static_cast<int>(static_cast<double>(m_dstRect.w) * 0.9) };
            int newDstRecth{ static_cast<int>(static_cast<double>(m_dstRect.h) * 0.9) };
            m_dstRect.x += static_cast<int>((1.0 * m_dstRect.w - newDstRectw) / 2.0);
            m_dstRect.y += static_cast<int>((1.0 * m_dstRect.h - newDstRecth) / 2.0);
            m_dstRect.w = newDstRectw;
            m_dstRect.h = newDstRecth;
            if (++m_implosionCount > 30)
            {
                pulledEnemiesExplode();
                m_boomSound.play();

                m_flying = false;
                m_isCooling = true;
                m_collided = false;
                m_flyingCount = 0;
                m_implosionCount = 0;
                for (auto& enemy : m_pulledEnemies)
                {
                    if (enemy && !enemy->isDying())
                    {
                        enemy->freeMovement();
                    }
                }
                m_pulledEnemies.clear();
            }
        }
    }
    else
    {
        setPos(player->getPos().getx() - 28.0, player->getPos().gety() - 28.0);
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

bool BlackHole::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
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
        if (m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider).first != Collider::NONE)
        {
            return true;
        }
    }
    return false;
}

void BlackHole::pullEnemies(std::vector<std::shared_ptr<Character>>& enemies)
{
    Vector2D<double> enemyRelPos;
    double enemyDistance;

    //pull any enemies in range that arent being pulled
    for (int i{ 0 }; i < static_cast<int>(enemies.size()); ++i)
    {
        //check for immobilised enemy to ensure they are not already being pulled
        if (enemies[i] && enemies[i]->isInUpdateRange() && !enemies[i]->isDying() && !enemies[i]->isImmobilised())
        {
            enemyRelPos = enemies[i]->getPos() + Vector2D<double>{(enemies[i]->getDstRect().w / 2.0), (enemies[i]->getDstRect().h / 2.0)} 
            - m_position - Vector2D<double>{78.0, 78.0};
            enemyDistance = enemyRelPos.magnitude();
            if (enemyDistance < m_pullRange)
            {
                enemies[i]->immobilise();
                enemies[i]->setVel(m_pullSpeed, 0.0);
                m_pulledEnemies.push_back(enemies[i]);
            }
        }
    }

    for (auto& enemy : m_pulledEnemies)
    {
        if (enemy && !enemy->isDying())
        {
            enemyRelPos = m_position + Vector2D<double>{78.0, 78.0} 
            - enemy->getPos() - Vector2D<double>{(enemy->getDstRect().w / 2.0), (enemy->getDstRect().h / 2.0)};
            enemy->setDirection(enemyRelPos.direction());
        }
    }
}

//searches for enemy closest to BlackHole and locks on
void BlackHole::pulledEnemiesCollideCheck()
{
    for (auto& enemy : m_pulledEnemies)
    {
        if (enemy && !enemy->isDying())
        {
            if (m_collider.collideCheck(enemy->getCollider()))
            {
                //enemy->removeHP(m_damage);
            }
        }
    }
}

void BlackHole::pulledEnemiesExplode()
{
    for (auto& enemy : m_pulledEnemies)
    {
        if (enemy && !enemy->isDying())
        {
            if (m_collider.collideCheck(enemy->getCollider()))
            {
                enemy->removeHP(10 * m_damage);
                enemy->addVel(50.0, 0);
                enemy->setDirection(MTRandom::getRandomDouble(0.0, 360.0));
            }
        }
    }
}

void BlackHole::motion()
{
    if (m_collided)
    {
        m_velocity.scale(0.0);
    }
}