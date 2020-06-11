#include "Spider.h"
#include "Player.h"

Spider::Spider(double xStartPos, double yStartPos, double xVel, double yVel, const char* fileName, int colliderWidth, int colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : GroundedEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount, damage)
{
    for (int i{ 0 }; i < m_spriteSheetCount; ++i)
    {
        m_spriteRects.push_back({ 20 * i, 0, 20, 15 });
    }

    m_climbSpeed = 10.0;
    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 20.0;
    m_walkAcceleration = 0.8;
    m_stopDeceleration = 0.9;

    m_dstRect.w = 100;
    m_dstRect.h = 75;
}

void Spider::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player)
{
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera
    if (m_position.getx() > 1.0 * camera.getx() - m_updateRange && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + m_updateRange
        && m_position.gety() > 1.0 * camera.gety() - m_updateRange && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + m_updateRange)
    {
        enemyControls(player);

        //edge check goes before map collision check to prevent vector subcript error when going off the edge
        if (edgeCheck(camera))
        {
            //collider position is moved after each function that can change character position
            setCollider();
        }

        bool airborne{ m_movement == AIRBORNE };
        bool collided{ sweepMapCollideCheck(map) };
        bool landed{ m_movement != AIRBORNE };

        //if enemy just landed from jumping
        if (airborne && landed)
        {
            m_landed = true;
        }

        if (!collided)
        {
            m_position.add(m_velocity);
            setCollider();
        }

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());

        motion();
        animateSprite();
        attackPlayer(player);
        cycleDamageFlash();
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
        break;

    case GroundedCharacter::CLIMB_UP:
        break;

    case GroundedCharacter::CLIMB_DOWN:
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
        if (m_position.getx() < player->getPos().getx() + 30 - m_collider.getHitBox().w)
        {
            m_movement = RIGHT;
            m_facingLeft = false;
            if (m_landed)
            {
                m_movement = STOP;
            }
        }
        else if (m_position.getx() > player->getPos().getx() + 70)
        {
            m_movement = LEFT;
            m_facingLeft = true;
            if (m_landed)
            {
                m_movement = STOP;
            }
        }
        else
        {
            m_movement = STOP;
        }

        if (!m_landed && MTRandom::getRandomInt(0, 99) < 1)
        {
            m_movement = AIRBORNE;
            //jumps with randomised velocity boost
            addVel(Vector2D<double>{(m_velocity.getx() > 0.0 ? 1.0 : -1.0)* MTRandom::getRandomDouble(0.0, 10.0), -MTRandom::getRandomDouble(20.0, 30.0)});
            m_jumped = true;
            m_spriteIndex = 8;
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
        m_movement = AIRBORNE;
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
                    yCollision = true;
                }
                break;

            case Collider::BOTTOM:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    yCollision = true;
                }
                break;

            case Collider::LEFT:
            case Collider::RIGHT:
                if (!xCollision)
                {
                    tempVel.xScale(result.second);
                    m_velocity.xScale(0);
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
                    yCollision = true;
                    //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
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
    if (m_animationStep >= 4)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 7)
        {
            m_spriteIndex = 0;
        }
    }
}

void Spider::animateSprite()
{
    if (m_jumped)
    {
        cycleJumpAnimation();
    }
    else if (m_landed)
    {
        cycleLandAnimation();
    }
    else
    {
        switch (m_movement)
        {
        case GroundedCharacter::AIRBORNE:
            m_spriteIndex = 12;
            break;
        case GroundedCharacter::LEFT:
        case GroundedCharacter::RIGHT:
        case GroundedCharacter::STOP:
            cycleWalkAnimation();
            break;
        default:
            break;
        }
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}