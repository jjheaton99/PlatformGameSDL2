#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight)
{}

GroundedCharacter::~GroundedCharacter()
{}

void GroundedCharacter::motion(double acceleration, double deceleration, double maxVel)
{
    if (m_movement == AIRBORNE && !(m_velocity.gety() > maxVel))
    {
        m_velocity.add(Vector2D<double>{0, Constants::g});
    }
    else if (m_movement == LEFT)
    {
        m_velocity.add(Vector2D<double>{-acceleration, 0});
        if (m_velocity.getx() < -m_xMaxSpeed)
        {
            setVel(-m_xMaxSpeed, 0);
        }
    }
    else if (m_movement == RIGHT)
    {
        m_velocity.add(Vector2D<double>{acceleration, 0});
        if (m_velocity.getx() > m_xMaxSpeed)
        {
            setVel(m_xMaxSpeed, 0);
        }
    }
    else
    {
        m_velocity.xScale(deceleration);
        if (std::abs(m_velocity.getx()) < 0.1)
        {
            m_velocity.xScale(0);
        }
    }
}

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

bool GroundedCharacter::checkForPlatforms(std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, SDL_Rect& characterCollider)
{
    for (int row{ characterRow }; row * tileSize <= characterCollider.y + characterCollider.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterCollider.x + characterCollider.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                //Checks for blocks directly below player whilst not colliding
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

void GroundedCharacter::mapCollideCheck(std::vector<std::vector<Tile>>& map)
{
    int tileSize{ map[0][0].getSize() };
    SDL_Rect characterCollider{ m_collider.getBox() };
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

    int xCollideCount{ 0 };
    int yCollideCount{ 0 };
    for (const auto& rect : collisionRects)
    {
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

        if ((yOverlap < xOverlap) && (characterCollider.y < rect.y))
        {
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y - characterCollider.h };
            m_velocity.yScale(0);
            m_movement = STOP;
            ++yCollideCount;
        }
        else if ((yOverlap < xOverlap) && (characterCollider.y > rect.y))
        {
            m_position = Vector2D<double>{ m_position.getx(), 1.0 * rect.y + rect.h };
            m_velocity.yScale(0);
            ++yCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterCollider.x < rect.x))
        {
            m_position.subtract(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0 * rect.x - characterCollider.w - 0.01, m_position.gety() };
            ++xCollideCount;
        }
        else if ((xOverlap < yOverlap) && (characterCollider.x > rect.x))
        {
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

    if (!checkForPlatforms(map, characterRow, characterColumn, tileSize, characterCollider))
    {
        m_movement = AIRBORNE;
    }
}