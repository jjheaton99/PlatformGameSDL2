#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_velocity{ xVel, yVel }
{}

Character::~Character()
{}

void Character::motion(double acceleration, double deceleration)
{
    if (m_movement == AIRBORNE)
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

void Character::edgeCheck()
{
    if (m_position.getx() < 0)
    {
        m_position.subtract(Vector2D<double>{m_position.getx(), 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.getx() > static_cast<std::int64_t>(Constants::screenWidth) - m_dstRect.w)
    {
        m_position.subtract(Vector2D<double>{m_position.getx() - static_cast<std::int64_t>(Constants::screenWidth) + m_dstRect.w, 0});
        setVel(0, m_velocity.gety());
    }
    //else if (m_position.gety() < 0)
    //{
    //   m_position.subtract(Vector2D<double>{0, m_position.gety()});
    //}
    else if (m_collider.getBox().y > static_cast<std::int64_t>(Constants::screenHeight) - m_dstRect.h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(Vector2D<double>{0, m_position.gety() - static_cast<std::int64_t>(Constants::screenHeight) + m_dstRect.h});
        setVel(m_velocity.getx(), 0);
    }
}

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

bool Character::checkForPlatforms(std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, SDL_Rect& characterCollider)
{
    for (int row{ characterRow }; row * tileSize <= characterCollider.y + characterCollider.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterCollider.x + characterCollider.w; ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                //Checks for blocks directly below player whilst not colliding
                if ((m_position.gety() == 1.0*map[row][column].getCollider().getBox().y - characterCollider.h)
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

void Character::mapCollideCheck(std::vector<std::vector<Tile>>& map)
{
    int tileSize{ map[0][0].getSize() };
    SDL_Rect characterCollider{ m_collider.getBox() };
    int characterColumn{ (characterCollider.x - (characterCollider.x % tileSize)) / tileSize };
    int characterRow{ (characterCollider.y - (characterCollider.y % tileSize)) / tileSize };
    //std::cout << characterRow << ' ' << characterColumn << '\n';

    std::vector<SDL_Rect> collisionRects{ getCollideTileBoxes(map, characterRow, characterColumn, tileSize, characterCollider) };
    std::cout << collisionRects.size() << '\n';

    for (const auto& rect : collisionRects)
    {
        double xOverlap{};
        if (characterCollider.x < rect.x)
        {
            xOverlap = 1.0*characterCollider.x + characterCollider.w - rect.x;
        }
        else if (characterCollider.x > rect.x)
        {
            xOverlap = 1.0*rect.x + rect.w - characterCollider.x;
        }

        double yOverlap{};
        if (characterCollider.y < rect.y)
        {
            yOverlap = 1.0*characterCollider.y + characterCollider.h - rect.y;
        }
        else if (characterCollider.y > rect.y)
        {
            yOverlap = 1.0*rect.y + rect.h - characterCollider.y;
        }

        if ((xOverlap < yOverlap) && (characterCollider.x < rect.x))
        {
            m_position.subtract(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0 * rect.x - characterCollider.w - 0.01, m_position.gety() };
            //Needs optimising!!!
            if (!checkForPlatforms(map, characterRow, characterColumn, tileSize, characterCollider))
            {
                m_velocity.xScale(0);
            }
        }
        else if ((xOverlap < yOverlap) && (characterCollider.x > rect.x))
        {
            m_position.add(Vector2D<double>{xOverlap, 0});
            //m_position = Vector2D<double>{ 1.0*rect.x + rect.w + 0.01, m_position.gety() };
            //Needs optimising!!!
            if (!checkForPlatforms(map, characterRow, characterColumn, tileSize, characterCollider))
            {
                m_velocity.xScale(0);
            }
        }
        else if ((yOverlap < xOverlap) && (characterCollider.y < rect.y))
        {
            m_position = Vector2D<double>{m_position.getx(), 1.0*rect.y - characterCollider.h};
            m_velocity.yScale(0);
            m_movement = STOP;
        }
        else if ((yOverlap < xOverlap) && (characterCollider.y > rect.y))
        {
            m_position = Vector2D<double>{m_position.getx(), 1.0*rect.y + rect.h};
            m_velocity.yScale(0);
        }
    }

    if (!checkForPlatforms(map, characterRow, characterColumn, tileSize, characterCollider))
    {
        m_movement = AIRBORNE;
    }
}