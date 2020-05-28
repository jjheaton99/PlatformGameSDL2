#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight, int hitPoints)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_hitPoints{hitPoints}, m_velocity{ xVel, yVel }
{}

Character::~Character()
{
    destroy();
}

void Character::getCollideTiles(const std::vector<std::vector<Tile>>& map, int characterRow, int characterColumn)
{
    //delete any existing hitboxes 
    m_solidColliders.clear();
    m_platformColliders.clear();

    //in case no ladder collisions are found
    m_collidingWithLadder = false;

    int startRow{ characterRow - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endRow{ characterRow + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().h / Constants::tileSize) + 2 };
    int startColumn{ characterColumn - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endColumn{ characterColumn + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().w / Constants::tileSize) + 2 };

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

    //checks all tiles that could be overlapping with character collision hitBox 
    //character row and column are the indices of the character's position in terms of tiles on the map
    for (int row{ startRow }; row <= endRow; ++row)
    {
        for (int column{ startColumn }; column <= endColumn; ++column)
        {
            if (map[row][column].getType() == Tile::LADDER)
            {
                //no && used because we dont want to collide check if we dont have to
                if (m_collider.collideCheck(map[row][column].getCollider()))
                {
                    m_collidingWithLadder = true;
                    m_ladderxPos = map[row][column].getCollider().getHitBox().x;
                }
            }

            switch (map[row][column].getType())
            {
            case Tile::SOLID:
                m_solidColliders.push_back({
                    map[row][column].getCollider(),
                    Collider::xOverlap(m_collider, map[row][column].getCollider()),
                    Collider::yOverlap(m_collider, map[row][column].getCollider()),
                    });
                break;
            case Tile::PLATFORM:
                m_platformColliders.push_back({
                    map[row][column].getCollider(),
                    Collider::xOverlap(m_collider, map[row][column].getCollider()),
                    Collider::yOverlap(m_collider, map[row][column].getCollider()),
                    });
                break;
            case Tile::BACKGROUND:
            default:
                break;
            }
        }
    }

}

void Character::setDirection(double direction)
{
    m_velocity.rotate(direction - m_velocity.direction());
}