#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight)
{}

GroundedCharacter::~GroundedCharacter()
{}

//adjusts velocity of character depending on state of motion
void GroundedCharacter::motion(double acceleration, double deceleration, double maxVel)
{
    if (m_movement == AIRBORNE && !(m_velocity.gety() > maxVel))
    {
        //grounded characters fall when airborne
        m_velocity.add(Vector2D<double>{0, Constants::g});
    }
    //velocity increased/decreased unless at max horizontal velocity
    else if (m_movement == LEFT)
    {   
        if (m_velocity.getx() < -m_xMaxSpeed)
        {
            m_velocity.xScale(deceleration + (deceleration / 10));
        }
        else
        {
            m_velocity.add(Vector2D<double>{-acceleration, 0});
        }
    }
    else if (m_movement == RIGHT)
    {
        if (m_velocity.getx() > m_xMaxSpeed)
        {
            m_velocity.xScale(deceleration + (deceleration / 10));
        }
        else
        {
            m_velocity.add(Vector2D<double>{acceleration, 0});
        }
    }
    else
    {
        //deceleration when stopped moving
        m_velocity.xScale(deceleration);
        if (std::abs(m_velocity.getx()) < 0.1)
        {
            m_velocity.xScale(0);
        }
    }
}

//resets velocity if colliding with edge of map
void GroundedCharacter::edgeCheck(Camera& camera)
{
    if (m_position.getx() < 0)
    {
        m_position.subtract(Vector2D<double>{m_position.getx(), 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.getx() > camera.getxBoundary() - m_dstRect.w)
    {
        m_position.subtract(Vector2D<double>{m_position.getx() - camera.getxBoundary() + m_dstRect.w, 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.gety() < 0)
    {
        m_position.subtract(Vector2D<double>{0, m_position.gety()});
        setVel(m_velocity.getx(), 0);
    }
    else if (m_collider.getBox().y > camera.getyBoundary() - m_dstRect.h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(Vector2D<double>{0, m_position.gety() - camera.getyBoundary() + m_dstRect.h});
        setVel(m_velocity.getx(), 0);
    }
}

//checks if character is standing on a solid map tile
bool GroundedCharacter::checkForPlatforms(std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, SDL_Rect& characterCollider)
{
    for (int row{ characterRow }; row * tileSize <= characterCollider.y + characterCollider.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterCollider.x + characterCollider.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                //Checks for blocks directly below character whilst not colliding
                if ((m_position.gety() == 1.0 * map[row][column].getCollider().getBox().y - characterCollider.h)
                    && (map[row][column].getCollider().getBox().x > (characterCollider.x - map[row][column].getCollider().getBox().w))
                    && (map[row][column].getCollider().getBox().x < (characterCollider.x + characterCollider.w)))
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
    SDL_Rect characterCollider{ m_collider.getBox() };
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ (characterCollider.x - (characterCollider.x % tileSize)) / tileSize };
    if (characterColumn > static_cast<int>(map[0].size() - 1))
    {
        m_position.subtract(m_velocity);
        return;
    }
    int characterRow{ (characterCollider.y - (characterCollider.y % tileSize)) / tileSize };
    if (characterRow > static_cast<int>(map.size() - 1))
    {
        m_position.subtract(m_velocity);
        return;
    }
    //std::cout << characterRow << ' ' << characterColumn << '\n';

    std::vector<SDL_Rect> collisionRects{ getCollideTileBoxes(map, characterRow, characterColumn, tileSize, characterCollider) };
    //std::cout << collisionRects.size() << '\n';

    //variables for counting collisions from x and y directions
    int xCollideCount{ 0 };
    int yCollideCount{ 0 };
    for (const auto& rect : collisionRects)
    {
        //overlap variables are the overlap of the character collider with tile collider in x or y direction
        double xOverlap{};
        if (characterCollider.x < rect.x)
        {
            xOverlap = 1.0 * characterCollider.x + characterCollider.w - rect.x;
        }
        else if (characterCollider.x > rect.x)
        {
            xOverlap = 1.0 * rect.x + rect.w - characterCollider.x;
        }
        else if (characterCollider.x = rect.x)
        {
            if (characterCollider.w > rect.w)
            {
                xOverlap = rect.w;
            }
            else
            {
                xOverlap = characterCollider.w;
            }
        }

        if (characterCollider.w > rect.w && xOverlap > rect.w)
        {
            xOverlap = rect.w;
        }
        else if (characterCollider.w < rect.w && xOverlap > characterCollider.w)
        {
            xOverlap = characterCollider.w;
        }

        double yOverlap{};
        if (characterCollider.y < rect.y)
        {
            yOverlap = 1.0 * characterCollider.y + characterCollider.h - rect.y;
        }
        else if (characterCollider.y > rect.y)
        {
            yOverlap = 1.0 * rect.y + rect.h - characterCollider.y;
        }
        else if (characterCollider.y == rect.y)
        {
            if (characterCollider.h > rect.h)
            {
                yOverlap = rect.h;
            }
            else
            {
                yOverlap = characterCollider.h;
            }
        }

        if (characterCollider.h > rect.h && yOverlap > rect.h)
        {
            yOverlap = rect.h;
        }
        else if (characterCollider.h < rect.h && yOverlap > characterCollider.h)
        {
            yOverlap = characterCollider.h;
        }

        //std::cout << xOverlap << "    " << yOverlap << '\n';

        //statements for handling four different collision cases
        //the collision direction is assumed to be the direction with smallest overlap
        //character is repositioned based on this
        if ((yOverlap < xOverlap) && (characterCollider.y < rect.y))
        {
            //adjustments for when character lands on top of a block
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y - characterCollider.h };
            m_velocity.yScale(0);
            m_movement = STOP;
            ++yCollideCount;
        }
        else if ((yOverlap < xOverlap) && (characterCollider.y > rect.y))
        {
            //jumping into overhead block
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y + rect.h };
            m_velocity.yScale(0);
            ++yCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterCollider.x < rect.x))
        {
            //colliding into the right of a block
            m_position.subtract(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0 * rect.x - characterCollider.w - 0.01, m_position.gety() };
            ++xCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterCollider.x > rect.x))
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
    if (!checkForPlatforms(map, characterRow, characterColumn, tileSize, characterCollider))
    {
        m_movement = AIRBORNE;
    }
}