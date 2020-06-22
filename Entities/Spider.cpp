#include "Spider.h"
#include "Player.h"

Spider::Spider(double xStartPos, double yStartPos, double xVel, double yVel, std::string fileName, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : GroundedEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount, damage)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 15 * i, 0, 15, 15 });
    }

    m_climbSpeed = 10.0;
    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 10.0;
    m_walkAcceleration = 0.8;
    m_stopDeceleration = 0.9;

    m_dstRect.w = 50;
    m_dstRect.h = 50;

    m_deathSound.load("Assets/Sounds/EnemySounds/spiderDeath.wav");

    m_type = EnemyType::SPIDER;
}

void Spider::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player)
{
    if (!m_killed)
    {
        if (m_hitPoints <= 0)
        {
            kill();
        }

        //only update when within certain distance from camera
        if (m_position.getx() > 1.0 * camera.getx() - m_updateRange && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + m_updateRange
            && m_position.gety() > 1.0 * camera.gety() - m_updateRange && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + m_updateRange)
        {
            if (!m_immobilised)
            {
                enemyControls(player);
            }
            else
            {
                m_movement = AIRBORNE;
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
                bool airborne{ m_movement == AIRBORNE };
                bool collided{ sweepMapCollideCheck(map) };
                bool landed{ m_movement != AIRBORNE };
                //if enemy just landed from jumping
                if (airborne && landed)
                {
                    m_landed = true;
                }
            }
            else
            {
                collided = immobilisedSweepMapCollideCheck(map);
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

            if (!m_immobilised)
            {
                motion();
            }
            animateSprite();
            cycleDamageFlash();

            if (m_projectile == ProjectileType::SPIDER)
            {
                m_projectile = ProjectileType::NONE;
            }
            if (++m_projectileCount > 100)
            {
                m_projectileCount = 0;
                m_projectile = ProjectileType::SPIDER;
                m_spiderShotSound.play();
            }
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

void Spider::setCollider()
{
    if (m_angle == 0.0)
    {
        m_collider.setDimensions(50, 30);
        m_collider.setPosition(m_position.getx(), m_position.gety() + 20);
    }
    else if (m_angle > 0.0)
    {
        m_collider.setDimensions(30, 50);
        m_collider.setPosition(m_position.getx(), m_position.gety());
    }
    else
    {
        m_collider.setDimensions(30, 50);
        m_collider.setPosition(m_position.getx() + 20, m_position.gety());
    }
}

//similar motion to player character
void Spider::motion()
{
    switch (m_movement)
    {
    case GroundedCharacter::AIRBORNE:
        if (m_velocity.gety() + Constants::g <= m_yMaxSpeed)
        {
            m_velocity.add(0, Constants::g);
        }
        else
        {
            m_velocity.yScale(0.0);
            m_velocity.add(0, m_yMaxSpeed);
        }
        break;

    case GroundedCharacter::LEFT:
        if (m_velocity.getx() - m_walkAcceleration >= -m_xMaxSpeed)
        {
            m_velocity.add(-m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() < -m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(-m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::RIGHT:
        if (m_velocity.getx() + m_walkAcceleration <= m_xMaxSpeed)
        {
            m_velocity.add(m_walkAcceleration, 0);
        }
        else if (m_velocity.getx() > m_xMaxSpeed)
        {
            m_velocity.xScale(0.9);
        }
        else
        {
            m_velocity.xScale(0.0);
            m_velocity.add(m_xMaxSpeed, 0);
        }
        break;

    case GroundedCharacter::CLIMB_STOP:
        m_velocity.scale(0.0);
        break;

    case GroundedCharacter::CLIMB_UP:
        setVel(0.0, -m_climbSpeed);
        break;

    case GroundedCharacter::CLIMB_DOWN:
        setVel(0.0, m_climbSpeed);
        break;

    case GroundedCharacter::STOP:
        m_velocity.xScale(m_stopDeceleration);
        if (std::abs(m_velocity.getx()) < 0.0001)
        {
            m_velocity.xScale(0);
        }
        break;

    default:
        break;
    }
}

void Spider::enemyControls(std::shared_ptr<Character> player)
{
    if (m_movement != AIRBORNE)
    {
        if (isClimbing())
        {
            if (m_position.getx() < player->getPos().getx() - 700)
            {
                m_movement = AIRBORNE;
                m_facingLeft = false;
                m_angle = 0.0;
                setVel(m_xMaxSpeed, -20.0);
            }
            else if (m_position.getx() > player->getPos().getx() + 750)
            {
                m_movement = AIRBORNE;
                m_facingLeft = true;
                m_angle = 0.0;
                setVel(-m_xMaxSpeed, -20.0);
            }
            else if (m_position.gety() < player->getPos().gety() - m_runAwayRange)
            {
                m_movement = CLIMB_DOWN;
                m_angle > 0.0 ? m_facingLeft = false : m_facingLeft = true;
            }
            else if (m_position.gety() < player->getPos().gety() - m_runAwayRange + 50)
            {
                m_movement = CLIMB_STOP;
                m_angle > 0.0 ? m_facingLeft = false : m_facingLeft = true;
            }
            else
            {
                m_movement = CLIMB_UP;
                m_angle > 0.0 ? m_facingLeft = true : m_facingLeft = false;
            }
        }

        else if (m_position.getx() < player->getPos().getx() - m_runAwayRange)
        {
            m_movement = RIGHT;
            m_facingLeft = false;
        }
        else if (m_position.getx() > player->getPos().getx() + 50 + m_runAwayRange)
        {
            m_movement = LEFT;
            m_facingLeft = true;
        }
        else if (m_position.getx() < player->getPos().getx() - m_runAwayRange + 50)
        {
            m_movement = STOP;
            m_velocity.scale(0.0);
            m_facingLeft = false;
        }
        else if (m_position.getx() > player->getPos().getx() + m_runAwayRange)
        {
            m_movement = STOP;
            m_velocity.scale(0.0);
            m_facingLeft = true;
        }
        else if (m_position.getx() < player->getPos().getx() + 25)
        {
            m_movement = LEFT;
            m_facingLeft = true;
        }
        else if (m_position.getx() > player->getPos().getx() + 75)
        {
            m_movement = RIGHT;
            m_facingLeft = false;
        }
    }
}

bool Spider::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ Tile::coordToMapIndex(m_collider.getHitBox().x) };
    int characterRow{ Tile::coordToMapIndex(m_collider.getHitBox().y) };

    //causes character to fall when stepping off platform or solid tile
    if (!checkForGround(map, characterRow, characterColumn, m_collider.getHitBox()))
    {
        if (m_angle == 0.0)
        {
            m_movement = AIRBORNE;
        }
        else if (m_angle > 0.0)
        {
            m_movement = AIRBORNE;
            setVel(-m_xMaxSpeed, 0.0);
            m_angle = 0.0;
        }
        else
        {
            m_movement = AIRBORNE;
            setVel(m_xMaxSpeed, 0.0);
            m_angle = 0.0;
        }
    }

    getCollideTiles(map, characterRow, characterColumn);
    //sort by greatest overlaps
    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const auto& a, const auto& b) {
        return ((std::get<1>(a) > std::get<1>(b) && std::get<1>(a) > std::get<2>(b)) || (std::get<2>(a) > std::get<1>(b) && std::get<2>(a) > std::get<2>(b)));
        }
    );

    bool xCollision{ false };
    bool yCollision{ false };

    Vector2D<double> tempVel{ m_velocity };

    for (auto& sweptCollider : m_solidColliders)
    {
        //if we have had collision in both directions, skip
        if (!xCollision || !yCollision)
        {
            auto result(m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider));

            switch (result.first)
            {
            case Collider::TOP:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    m_movement = STOP;
                    m_angle = 0.0;
                    yCollision = true;
                }
                break;

            case Collider::BOTTOM:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    m_movement = CLIMB_STOP;
                    m_angle = 0.0;
                    yCollision = true;
                }
                break;

            case Collider::LEFT:
                if (!xCollision)
                {
                    tempVel.xScale(result.second);
                    m_velocity.xScale(0);
                    m_movement = CLIMB_STOP;
                    m_angle = -90.0;
                    xCollision = true;
                }
                break;
            case Collider::RIGHT:
                if (!xCollision)
                {
                    tempVel.xScale(result.second);
                    m_velocity.xScale(0);
                    m_movement = CLIMB_STOP;
                    m_angle = 90.0;
                    xCollision = true;
                }
                break;

            case Collider::NONE:
            default:
                break;
            }
        }
    }

    for (auto& sweptCollider : m_platformColliders)
    {
        if (!yCollision)
        {
            auto result{ m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider) };

            if (result.first == Collider::TOP)
            {
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    m_movement = STOP;
                    m_angle = 0.0;
                    yCollision = true;
                }
            }
        }
    }

    if (xCollision || yCollision)
    {
        m_position.add(tempVel);
        setCollider();
    }

    return xCollision || yCollision;
}

void Spider::cycleWalkAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 6)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 5)
        {
            m_spriteIndex = 0;
        }
    }
}

void Spider::cycleIdleAnimation()
{
    ++m_animationStep;
    if (m_animationStep >= 6)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 10)
        {
            m_spriteIndex = 6;
        }
    }
}

void Spider::animateSprite()
{
    if (m_movement == AIRBORNE)
    {
        m_spriteIndex = 6;
    }
    else if (m_movement != STOP && m_movement != CLIMB_STOP)
    {
        cycleWalkAnimation();
    }
    else
    {
        cycleIdleAnimation();
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}

bool Spider::checkForGround(const std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, const Collider::DoubleRect& characterColliderBox)
{
    if (m_angle == 0.0)
    {
        for (int row{ characterRow }; 1.0 * row * Constants::tileSize <= characterColliderBox.y + characterColliderBox.h && row < static_cast<int>(map.size()); ++row)
        {
            for (int column{ characterColumn }; 1.0 * column * Constants::tileSize <= characterColliderBox.x + characterColliderBox.w && column < static_cast<int>(map[0].size()); ++column)
            {
                if (map[row][column].getType() == Tile::SOLID || map[row][column].getType() == Tile::PLATFORM)
                {
                    if ((map[row][column].getCollider().getHitBox().y + Constants::tileSize > characterColliderBox.y + characterColliderBox.h)
                        && (map[row][column].getCollider().getHitBox().x + Constants::tileSize > characterColliderBox.x)
                        && (map[row][column].getCollider().getHitBox().x < characterColliderBox.x + characterColliderBox.w))
                    {
                        return true;
                    }
                }
            }
        }
    }
    
    else if (m_angle > 0.0)
    {
        for (int row{ characterRow - 1 }; 1.0 * row * Constants::tileSize <= characterColliderBox.y + characterColliderBox.h && row < static_cast<int>(map.size()); ++row)
        {
            for (int column{ characterColumn - 1}; 1.0 * column * Constants::tileSize <= characterColliderBox.x + characterColliderBox.w && column < static_cast<int>(map[0].size()); ++column)
            {
                if (map[row][column].getType() == Tile::SOLID)
                {
                    if ((map[row][column].getCollider().getHitBox().x < characterColliderBox.x)
                        && (map[row][column].getCollider().getHitBox().y + Constants::tileSize > characterColliderBox.y)
                        && (map[row][column].getCollider().getHitBox().y < characterColliderBox.y + characterColliderBox.h))
                    {
                        return true;
                    }
                }
            }
        }
    }

    else
    {
        for (int row{ characterRow }; 1.0 * row * Constants::tileSize <= characterColliderBox.y + characterColliderBox.h && row < static_cast<int>(map.size()); ++row)
        {
            for (int column{ characterColumn }; 1.0 * column * Constants::tileSize <= characterColliderBox.x + characterColliderBox.w && column < static_cast<int>(map[0].size()); ++column)
            {
                if (map[row][column].getType() == Tile::SOLID)
                {
                    if ((map[row][column].getCollider().getHitBox().x + Constants::tileSize > characterColliderBox.x + characterColliderBox.w)
                        && (map[row][column].getCollider().getHitBox().y + Constants::tileSize > characterColliderBox.y)
                        && (map[row][column].getCollider().getHitBox().y < characterColliderBox.y + characterColliderBox.h))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}