#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_velocity{ xVel, yVel }
{}

Character::~Character()
{}

//Checks tiles in the vicinity of the character to find collisions
std::vector<SDL_Rect> Character::getCollideTileBoxes(std::vector<std::vector<Tile>>& map, int characterRow, 
    int characterColumn, int tileSize, SDL_Rect& characterCollider)
{
    std::vector<SDL_Rect> collisionRects;

    for (int row{ characterRow }; row * tileSize <= characterCollider.y + characterCollider.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterCollider.x + characterCollider.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                if (m_collider.collideCheck(map[row][column].getCollider()))
                {
                    collisionRects.push_back(map[row][column].getCollider().getBox());
                }
            }
        }
    }

    return collisionRects;
}
