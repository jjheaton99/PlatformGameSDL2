#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount)
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
    if ((m_collidingWithLadder && isClimbing()))
    {
        return true;
    }
    
    for (int row{ characterRow }; 1.0 * row * Constants::tileSize <= characterColliderBox.y + characterColliderBox.h && row < static_cast<int>(map.size()); ++row)
    {
        for (int column{ characterColumn }; 1.0 * column * Constants::tileSize <= characterColliderBox.x + characterColliderBox.w && column < static_cast<int>(map[0].size()); ++column)
        {
            if ((map[row][column].getType() == Tile::SOLID && !m_crouched) || map[row][column].getType() == Tile::PLATFORM)
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

    return false;
}

bool GroundedCharacter::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
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