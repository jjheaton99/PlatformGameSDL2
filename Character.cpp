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
    else if (m_position.gety() < 0)
    {
        m_position.subtract(Vector2D<double>{0, m_position.gety()});
    }
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

SDL_Rect Character::getCollideTile(std::vector<std::vector<Tile>>& map)
{
    for (Uint8 row{ 0 }; row < map.size(); ++row)
    {
        for (Uint8 column{ 0 }; column < map[0].size(); ++column)
        {
            if (map[row][column].getType() == Tile::SOLID)
            {
                if (m_collider.collideCheck(map[row][column].getCollider()))
                {
                    return map[row][column].getCollider().getBox();
                }
            }
        }
    }

    return {-1, -1, -1, -1};
}

void Character::mapCollideCheck(std::vector<std::vector<Tile>>& map)
{
    SDL_Rect collideRect{ getCollideTile(map) };

    if (collideRect.x != -1)
    {
        if ((m_movement == AIRBORNE) && (m_velocity.gety() > 0))
        {
            m_position.subtract(Vector2D<double>{0, static_cast<double>(m_dstRect.h) - static_cast<double>(collideRect.y) + m_position.gety() + 1});
            m_movement = STOP;
            setVel(m_velocity.getx(), 0);
        }

        else if ((m_movement == AIRBORNE) && (m_velocity.gety() < 0))
        {
            m_position.add(Vector2D<double>{0, static_cast<double>(collideRect.h) -  m_position.gety() + static_cast<double>(collideRect.y) + 1});
            setVel(m_velocity.getx(), -m_velocity.gety());
        }
        
        std::cout << "Collided" << '\n';
    }
}