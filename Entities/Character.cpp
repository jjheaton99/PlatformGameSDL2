#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_velocity{ xVel, yVel }
{}

Character::~Character()
{
    destroy();
}

//checks tiles in the vicinity of the character to find collisions and stores colliding tiles in a vector
void Character::collideTileHitBoxes(const std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, const SDL_Rect& characterColliderBox)
{
    //delete any existing hitboxes 
    m_solidHitBoxes.clear();
    m_platformHitBoxes.clear();

    //in case no ladder collisions are found
    m_collidingWithLadder = false;

    //checks all tiles that could be overlapping with character collision hitBox 
    for (int row{ characterRow }; row * tileSize <= characterColliderBox.y + characterColliderBox.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterColliderBox.x + characterColliderBox.w; ++column)
        {
            if (m_collider.collideCheck(map[row][column].getCollider()))
            {
                switch (map[row][column].getType())
                {
                case Tile::SOLID:
                    m_solidHitBoxes.push_back(map[row][column].getCollider().getHitBox());
                    break;

                case Tile::PLATFORM:
                    m_platformHitBoxes.push_back(map[row][column].getCollider().getHitBox());
                    break;

                case Tile::LADDER:
                    m_collidingWithLadder = true;
                    m_ladderxPos = map[row][column].getCollider().getHitBox().x;
                    break;

                default:
                    break;
                }
            }
        }
    }
}
