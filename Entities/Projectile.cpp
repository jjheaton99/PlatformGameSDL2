#include "Projectile.h"

Projectile::Projectile(std::string fileName, double xPos, double yPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int damage)
    : GameObject(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight), m_damage{ damage }
{}

Projectile::~Projectile()
{}

void Projectile::getCollideTiles(const std::vector<std::vector<Tile>>& map, int projectileRow, int projectileColumn)
{
    //delete any existing hitboxes 
    m_solidColliders.clear();

    int startRow{ projectileRow - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endRow{ projectileRow + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().h / Constants::tileSize) + 2 };
    int startColumn{ projectileColumn - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endColumn{ projectileColumn + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().w / Constants::tileSize) + 2 };

    //check rows and cols within map
    if (startRow < 0)
    {
        startRow = 0;
    }
    if (endRow > static_cast<int>(map.size()) - 1)
    {
        endRow = map.size() - 1;
    }
    if (startColumn < 0)
    {
        startColumn = 0;
    }
    if (endColumn > static_cast<int>(map[0].size()) - 1)
    {
        endColumn = map[0].size() - 1;
    }

    for (int row{ startRow }; row <= endRow; ++row)
    {
        for (int column{ startColumn }; column <= endColumn; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                m_solidColliders.push_back({
                    map[row][column].getCollider(),
                    Collider::xOverlap(m_collider, map[row][column].getCollider()),
                    Collider::yOverlap(m_collider, map[row][column].getCollider()),
                });
            }
        }
    }
}

bool Projectile::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    int projectileColumn{ static_cast<int>(m_collider.getHitBox().x / Constants::tileSize) };
    int projectileRow{ static_cast<int>(m_collider.getHitBox().y / Constants::tileSize) };

    getCollideTiles(map, projectileRow, projectileColumn);

    for (auto& sweptCollider : m_solidColliders)
    {
        if (m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider).first != Collider::NONE)
        {
            return true;
        }
    }

    return false;
}