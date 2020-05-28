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

    for (auto& sweptCollider : m_solidColliders)
    {
        if (m_collider.sweptAABBdeflect(1.0, sweptCollider, m_position, m_velocity, Vector2D<double>{0.0, 0.0} ))
        {
            setCollider();
            ++collideCount;
        }
    }

    return collideCount > 0;
}

