#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_velocity{ xVel, yVel }
{}

Character::~Character()
{}

//checks tiles in the vicinity of the character to find collisions and stores colliding tiles in a vector
void Character::getCollideTileHitBoxes(std::vector<std::vector<Tile>>& map, int characterRow, 
    int characterColumn, int tileSize, SDL_Rect& characterColliderBox)
{
    //delete any existing hitboxes 
    m_tileHitBoxes.clear();

    //checks all tiles that could be overlapping with character collision hitBox 
    for (int row{ characterRow }; row * tileSize <= characterColliderBox.y + characterColliderBox.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterColliderBox.x + characterColliderBox.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                if (m_collider.collideCheck(map[row][column].getCollider()))
                {
                    m_tileHitBoxes.push_back(map[row][column].getCollider().getHitBox());
                }
            }
        }
    }
}
