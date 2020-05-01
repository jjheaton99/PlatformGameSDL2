#include "Character.h"

Character::Character(const char* fileName, float xStartPos, float yStartPos, float xVel, float yVel, int colliderWidth, int colliderHeight)
    : GameObject(fileName, xStartPos, yStartPos, colliderWidth, colliderHeight), m_velocity{ xVel, yVel }
{}

Character::~Character()
{}

void Character::motion(float acceleration, float deceleration)
{
    if (m_movement == AIRBORNE)
    {
        m_velocity.add(Vector2D<float>{0, Constants::g});
    }
    else if (m_movement == LEFT)
    {
        m_velocity.add(Vector2D<float>{-acceleration, 0});
        if (m_velocity.getx() < -m_xMaxSpeed)
        {
            setVel(-m_xMaxSpeed, 0);
        }
    }
    else if (m_movement == RIGHT)
    {
        m_velocity.add(Vector2D<float>{acceleration, 0});
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
        m_position.subtract(Vector2D<float>{m_position.getx(), 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.getx() > static_cast<std::int64_t>(Constants::screenWidth) - m_dstRect.w)
    {
        m_position.subtract(Vector2D<float>{m_position.getx() - static_cast<std::int64_t>(Constants::screenWidth) + m_dstRect.w, 0});
        setVel(0, m_velocity.gety());
    }
    else if (m_position.gety() < 0)
    {
        m_position.subtract(Vector2D<float>{0, m_position.gety()});
    }
    else if (m_collider.getBox().y > static_cast<std::int64_t>(Constants::screenHeight) - m_dstRect.h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(Vector2D<float>{0, m_position.gety() - static_cast<std::int64_t>(Constants::screenHeight) + m_dstRect.h});
        setVel(m_velocity.getx(), 0);
    }
}

std::vector<SDL_Rect> Character::getCollideTileBoxes(std::vector<std::vector<Tile>>& map)
{
    std::vector<SDL_Rect> collisionRects;
    for (Uint8 row{ 0 }; row < map.size(); ++row)
    {
        for (Uint8 column{ 0 }; column < map[0].size(); ++column)
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

bool Character::checkForPlatforms(std::vector<std::vector<Tile>>& map)
{
    for (Uint8 row{ 0 }; row < map.size(); ++row)
    {
        for (Uint8 column{ 0 }; column < map[0].size(); ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                if ((m_position.gety() == map[row][column].getCollider().getBox().y - m_collider.getBox().h)
                    && (map[row][column].getCollider().getBox().x > (m_collider.getBox().x - map[row][column].getCollider().getBox().w))
                    && (map[row][column].getCollider().getBox().x < (m_collider.getBox().x + m_collider.getBox().w)))
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
    std::vector<SDL_Rect> collisionRects{ getCollideTileBoxes(map) };

    for (const auto& rect : collisionRects)
    {
        float xOverlap{};
        if (m_collider.getBox().x < rect.x)
        {
            xOverlap = static_cast<float>(m_collider.getBox().x + m_collider.getBox().w - rect.x);
        }
        else if (m_collider.getBox().x > rect.x)
        {
            xOverlap = static_cast<float>(rect.x + rect.w - m_collider.getBox().x);
        }

        float yOverlap{};
        if (m_collider.getBox().y < rect.y)
        {
            yOverlap = static_cast<float>(m_collider.getBox().y + m_collider.getBox().h - rect.y);
        }
        else if (m_collider.getBox().y > rect.y)
        {
            yOverlap = static_cast<float>(rect.y + rect.h - m_collider.getBox().y);
        }

        if ((xOverlap < yOverlap) && (m_collider.getBox().x < rect.x))
        {
            m_position.subtract(Vector2D<float>{xOverlap, 0});
            m_velocity.xScale(0);
        }
        else if ((xOverlap < yOverlap) && (m_collider.getBox().x > rect.x))
        {
            m_position.add(Vector2D<float>{xOverlap, 0});
            m_velocity.xScale(0);
        }
        else if ((yOverlap < xOverlap) && (m_collider.getBox().y < rect.y))
        {
            m_position = Vector2D<float>{m_position.getx(), static_cast<float>(rect.y - m_collider.getBox().h)};
            m_velocity.yScale(0);
            m_movement = STOP;
        }
        else if ((yOverlap < xOverlap) && (m_collider.getBox().y > rect.y))
        {
            m_position = Vector2D<float>{m_position.getx(), static_cast<float>(rect.y + rect.h)};
            m_velocity.yScale(0);
        }
    }

    if (!checkForPlatforms(map))
    {
        m_movement = AIRBORNE;
    }

    /*
    if (collideRect.x != -1)
    {
        if ((m_movement == AIRBORNE) && (m_velocity.gety() > 0))
        {
            m_position.subtract(Vector2D<float>{0, static_cast<float>(m_dstRect.h) - static_cast<float>(collideRect.y) + m_position.gety() + 1});
            m_movement = STOP;
            setVel(m_velocity.getx(), 0);
        }

        else if ((m_movement == AIRBORNE) && (m_velocity.gety() < 0))
        {
            m_position.add(Vector2D<float>{0, static_cast<float>(collideRect.h) -  m_position.gety() + static_cast<float>(collideRect.y) + 1});
            setVel(m_velocity.getx(), -m_velocity.gety());
        }
        
        std::cout << "Collided" << '\n';
    }
    */
}