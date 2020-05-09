#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight)
{}

GroundedCharacter::~GroundedCharacter()
{}

//resets velocity if colliding with edge of map
bool GroundedCharacter::edgeCheck(Camera& camera)
{
    if (m_collider.getHitBox().x <= 0)
    {
        m_position.subtract(Vector2D<double>{1.0 * m_collider.getHitBox().x - 1.0, 0});
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().x >= camera.getxBoundary() - m_collider.getHitBox().w)
    {
        m_position.subtract(Vector2D<double>{1.0 * m_collider.getHitBox().x - 1.0 * camera.getxBoundary() + 1.0 * m_collider.getHitBox().w + 1.0, 0});
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().y <= 0)
    {
        m_position.subtract(Vector2D<double>{0, 1.0 * m_collider.getHitBox().y - 1.0});
        setVel(m_velocity.getx(), 0);
        return true;
    }
    else if (m_collider.getHitBox().y >= camera.getyBoundary() - m_collider.getHitBox().h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(Vector2D<double>{0, 1.0 * m_collider.getHitBox().y - 1.0 * camera.getyBoundary() + 1.0 * m_collider.getHitBox().h + 1.0});
        setVel(m_velocity.getx(), 0);
        return true;
    }
    return false;
}

//checks if character is standing on a solid map tile
bool GroundedCharacter::checkForGround(std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, SDL_Rect& characterColliderBox)
{
    for (int row{ characterRow }; row * tileSize <= characterColliderBox.y + characterColliderBox.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterColliderBox.x + characterColliderBox.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                //Checks for blocks directly below character whilst not colliding
                if ((m_position.gety() == 1.0 * map[row][column].getCollider().getHitBox().y - characterColliderBox.h)
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

//checks for collision with solid map tiles and adjusts position/velocity
void GroundedCharacter::mapCollideCheck(std::vector<std::vector<Tile>>& map)
{
    int tileSize{ map[0][0].getSize() };
    SDL_Rect characterColliderBox{ m_collider.getHitBox() };
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ (characterColliderBox.x - (characterColliderBox.x % tileSize)) / tileSize };
    int characterRow{ (characterColliderBox.y - (characterColliderBox.y % tileSize)) / tileSize };
    //std::cout << characterRow << ' ' << characterColumn << '\n';

    getCollideTileHitBoxes(map, characterRow, characterColumn, tileSize, characterColliderBox);
    //std::cout << m_tileHitBoxes.size() << '\n';

    //variables for counting collisions from x and y directions
    int xCollideCount{ 0 };
    int yCollideCount{ 0 };
    for (const auto& rect : m_tileHitBoxes)
    {
        //overlap variables are the overlap of the character collider with tile collider in x or y direction
        double xOverlap{};
        if (characterColliderBox.x < rect.x)
        {
            xOverlap = 1.0 * characterColliderBox.x + characterColliderBox.w - rect.x;
        }
        else if (characterColliderBox.x > rect.x)
        {
            xOverlap = 1.0 * rect.x + rect.w - characterColliderBox.x;
        }
        else if (characterColliderBox.x = rect.x)
        {
            if (characterColliderBox.w > rect.w)
            {
                xOverlap = rect.w;
            }
            else
            {
                xOverlap = characterColliderBox.w;
            }
        }

        if (characterColliderBox.w > rect.w && xOverlap > rect.w)
        {
            xOverlap = rect.w;
        }
        else if (characterColliderBox.w < rect.w && xOverlap > characterColliderBox.w)
        {
            xOverlap = characterColliderBox.w;
        }

        double yOverlap{};
        if (characterColliderBox.y < rect.y)
        {
            yOverlap = 1.0 * characterColliderBox.y + characterColliderBox.h - rect.y;
        }
        else if (characterColliderBox.y > rect.y)
        {
            yOverlap = 1.0 * rect.y + rect.h - characterColliderBox.y;
        }
        else if (characterColliderBox.y == rect.y)
        {
            if (characterColliderBox.h > rect.h)
            {
                yOverlap = rect.h;
            }
            else
            {
                yOverlap = characterColliderBox.h;
            }
        }

        if (characterColliderBox.h > rect.h && yOverlap > rect.h)
        {
            yOverlap = rect.h;
        }
        else if (characterColliderBox.h < rect.h && yOverlap > characterColliderBox.h)
        {
            yOverlap = characterColliderBox.h;
        }

        //std::cout << xOverlap << "    " << yOverlap << '\n';

        //statements for handling four different collision cases
        //the collision direction is assumed to be the direction with smallest overlap
        //character is repositioned based on this
        if ((yOverlap < xOverlap) && (characterColliderBox.y < rect.y))
        {
            //adjustments for when character lands on top of a block
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y - characterColliderBox.h };
            m_velocity.yScale(0);
            m_movement = STOP;
            ++yCollideCount;
        }
        else if ((yOverlap < xOverlap) && (characterColliderBox.y > rect.y))
        {
            //jumping into overhead block
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y + rect.h };
            m_velocity.yScale(0);
            ++yCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterColliderBox.x < rect.x))
        {
            //colliding into the right of a block
            m_position.subtract(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0 * rect.x - characterColliderBox.w - 0.01, m_position.gety() };
            ++xCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterColliderBox.x > rect.x))
        {
            //colliding into left of a block
            m_position.add(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0*rect.x + rect.w, m_position.gety() };
            ++xCollideCount;
        }
    }

    //Stops motion when colliding with wall and prevents momentum stopping issue when landing on flat ground
    if (xCollideCount != 0 && yCollideCount == 0)
    {
        m_velocity.xScale(0);
    }

    //causes character to fall when stepping off solid tile platform
    if (!checkForGround(map, characterRow, characterColumn, tileSize, characterColliderBox))
    {
        m_movement = AIRBORNE;
    }
}