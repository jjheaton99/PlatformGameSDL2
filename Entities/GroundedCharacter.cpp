#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight)
{}

GroundedCharacter::~GroundedCharacter()
{}

//resets velocity if colliding with edge of map
bool GroundedCharacter::edgeCheck(const Camera& camera)
{
    if (m_collider.getHitBox().x <= 0)
    {
        m_position.subtract(1.0 * m_collider.getHitBox().x - 1.0, 0);
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().x >= camera.getxBoundary() - m_collider.getHitBox().w)
    {
        m_position.subtract(1.0 * m_collider.getHitBox().x - 1.0 * camera.getxBoundary() + 1.0 * m_collider.getHitBox().w + 1.0, 0);
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().y <= 0)
    {
        m_position.subtract(0, 1.0 * m_collider.getHitBox().y - 1.0);
        setVel(m_velocity.getx(), 0);
        return true;
    }
    else if (m_collider.getHitBox().y >= camera.getyBoundary() - m_collider.getHitBox().h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(0, 1.0 * m_collider.getHitBox().y - 1.0 * camera.getyBoundary() + 1.0 * m_collider.getHitBox().h + 1.0);
        setVel(m_velocity.getx(), 0);
        return true;
    }
    return false;
}

//checks if character is standing on a solid map tile
bool GroundedCharacter::checkForGround(const std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, const Collider::DoubleRect& characterColliderBox)
{
    //allows characters to continue climbing when in contact with ladders
    if (m_collidingWithLadder)
    {
        return true;
    }

    for (int row{ characterRow }; 1.0 * row * Constants::tileSize <= characterColliderBox.y + characterColliderBox.h && row < static_cast<int>(map.size()); ++row)
    {
        for (int column{ characterColumn }; 1.0 * column * Constants::tileSize <= characterColliderBox.x + characterColliderBox.w && column < static_cast<int>(map[0].size()); ++column)
        {
            if ((map[row][column].getType() == Tile::SOLID && !m_crouched) || map[row][column].getType() == Tile::PLATFORM)
            {
                //Checks for blocks directly below character whilst not colliding
                if ((m_movement != AIRBORNE)
                    && (map[row][column].getCollider().getHitBox().x > (characterColliderBox.x - map[row][column].getCollider().getHitBox().w))
                    && (map[row][column].getCollider().getHitBox().x < (characterColliderBox.x + characterColliderBox.w)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool GroundedCharacter::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ static_cast<int>(m_collider.getHitBox().x  / Constants::tileSize) };
    int characterRow{ static_cast<int>(m_collider.getHitBox().y / Constants::tileSize) };

    getCollideTiles(map, characterRow, characterColumn);

    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const Collider& a, const Collider& b) {
        double axOverlap{ Collider::axisBoxOverlap(m_collider.getHitBox().x, a.getHitBox().x, m_collider.getHitBox().w, a.getHitBox().w) };
        double bxOverlap{ Collider::axisBoxOverlap(m_collider.getHitBox().x, b.getHitBox().x, m_collider.getHitBox().w, b.getHitBox().w) };
        double ayOverlap{ Collider::axisBoxOverlap(m_collider.getHitBox().y, a.getHitBox().y, m_collider.getHitBox().h, a.getHitBox().h) };
        double byOverlap{ Collider::axisBoxOverlap(m_collider.getHitBox().y, b.getHitBox().y, m_collider.getHitBox().h, b.getHitBox().h) };
        return ((axOverlap > bxOverlap && axOverlap > byOverlap) || (ayOverlap > bxOverlap && ayOverlap > byOverlap));
        }
    );

    int collideCount{ 0 };
    for (const auto& collider : m_solidColliders)
    {
        auto collideResult{ m_collider.tileCollideCheck(m_velocity, collider) };
        switch (collideResult.first)
        {
        case Collider::TOP:
            m_velocity.yScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.yScale(0);
            m_movement = STOP;
            ++collideCount;
            //std::cout << "top" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::BOTTOM:
            m_velocity.yScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.yScale(0);
            ++collideCount;
            //std::cout << "bottom" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::LEFT:
            m_velocity.xScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.xScale(0);
            ++collideCount;
            //std::cout << "left" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::RIGHT:
            m_velocity.xScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.xScale(0);
            ++collideCount;
            //std::cout << "right" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::NONE:
        default:
            break;
        }
    }

    for (const auto& collider : m_platformColliders)
    {
        auto collideResult{ m_collider.tileCollideCheck(m_velocity, collider) };
        if (collideResult.first == Collider::TOP && !m_crouched && !m_hasCrouched)
        {
            m_velocity.yScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.yScale(0);
            m_movement = STOP;
            ++collideCount;
            //std::cout << "platform" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
        }
    }

    //causes character to fall when stepping off platform or solid tile
    if (!checkForGround(map, characterRow, characterColumn, m_collider.getHitBox()))
    {
        m_movement = AIRBORNE;
    }

    return collideCount > 0;
}

void GroundedCharacter::crouch() 
{ 
    m_crouched = true;
    m_hasCrouched = true;
}