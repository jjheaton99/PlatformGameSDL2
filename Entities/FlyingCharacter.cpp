#include "FlyingCharacter.h"


FlyingCharacter::FlyingCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight, int hitPoints)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints)
{}

FlyingCharacter::~FlyingCharacter()
{}


bool FlyingCharacter::edgeCheck(const Camera& camera)
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
        m_position.subtract(0, 1.0 * m_collider.getHitBox().y - 1.0 * camera.getyBoundary() + 1.0 * m_collider.getHitBox().h + 1.0);
        setVel(m_velocity.getx(), 0);
        return true;
    }
    return false;
}

bool FlyingCharacter::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ static_cast<int>(m_collider.getHitBox().x / Constants::tileSize) };
    int characterRow{ static_cast<int>(m_collider.getHitBox().y / Constants::tileSize) };

    getCollideTiles(map, characterRow, characterColumn);

    //sort by greatest overlaps
    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const auto& a, const auto& b) {
        return ((std::get<1>(a) > std::get<1>(b) && std::get<1>(a) > std::get<2>(b)) || (std::get<2>(a) > std::get<1>(b) && std::get<2>(a) > std::get<2>(b)));
        }
    );

    int collideCount{ 0 };
    double deflectionFactor{ -1.0 };

    for (const auto& collider : m_solidColliders)
    {
        auto collideResult{ m_collider.tileCollideCheck(m_velocity, collider) };
        switch (collideResult.first)
        {
        case Collider::TOP:
            m_velocity.yScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.yScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "top" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::BOTTOM:
            m_velocity.yScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.yScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "bottom" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::LEFT:
            m_velocity.xScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.xScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "left" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

        case Collider::RIGHT:
            m_velocity.xScale(collideResult.second);
            m_position.add(m_velocity);
            m_velocity.xScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "right" << '\n';
            //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
            break;

            //for overlap collisions we subtract velocity to move character out of block
        case Collider::OVERLAP_TOP:
            m_velocity.yScale(collideResult.second);
            m_position.subtract(m_velocity);
            m_velocity.yScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "otop" << '\n';
            break;

        case Collider::OVERLAP_BOTTOM:
            m_velocity.yScale(collideResult.second);
            m_position.subtract(m_velocity);
            m_velocity.yScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "obottom" << '\n';
            break;

        case Collider::OVERLAP_LEFT:
            m_velocity.xScale(collideResult.second);
            m_position.subtract(m_velocity);
            m_velocity.xScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "oleft" << '\n';
            break;

        case Collider::OVERLAP_RIGHT:
            m_velocity.xScale(collideResult.second);
            m_position.subtract(m_velocity);
            m_velocity.xScale(deflectionFactor);
            setCollider();
            ++collideCount;
            //std::cout << "oright" << '\n';
            break;

        case Collider::NONE:
        default:
            break;
        }
    }

    return collideCount > 0;
}

