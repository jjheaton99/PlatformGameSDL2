#include "Character.h"

Character::Character(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight, int hitPoints)
    : GameObject(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight), m_maxHitPoints{hitPoints}
{
    m_hitPoints = m_maxHitPoints;
}

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

void Character::cameraDraw(const Camera& camera) const
{
    //objects off the screen are not rendered
    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, 0, nullptr, SDL_FLIP_NONE);

        if (m_hitPoints < m_maxHitPoints)
        {
            double healthFraction{ (static_cast<double>(m_hitPoints) / m_maxHitPoints) };
            SDL_Rect healthBar{ m_dstRect.x - camera.getx() + (m_dstRect.w / 2) - 25,
                m_dstRect.y - camera.gety() + m_dstRect.h + 10, static_cast<int>(healthFraction * 50.0), 10 };
            SDL_Rect healthBarOutline{ m_dstRect.x - camera.getx() + (m_dstRect.w / 2) - 25,
                m_dstRect.y - camera.gety() + m_dstRect.h + 10, 50, 10 };

            if (healthFraction > 0.75)
            {
                SDL_SetRenderDrawColor(g_renderer, 0, 70, 0, 255);
            }
            else if (healthFraction > 0.5)
            {
                SDL_SetRenderDrawColor(g_renderer, 60, 70, 0, 255);
            }
            else if (healthFraction > 0.25)
            {
                SDL_SetRenderDrawColor(g_renderer, 70, 43, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(g_renderer, 70, 0, 0, 255);
            }
            SDL_RenderFillRect(g_renderer, &healthBarOutline);

            if (healthFraction > 0.75)
            {
                SDL_SetRenderDrawColor(g_renderer, 0, 230, 0, 255);
            }
            else if (healthFraction > 0.5)
            {
                SDL_SetRenderDrawColor(g_renderer, 200, 230, 0, 255);
            }
            else if (healthFraction > 0.25)
            {
                SDL_SetRenderDrawColor(g_renderer, 230, 140, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(g_renderer, 230, 0, 0, 255);
            }
            SDL_RenderFillRect(g_renderer, &healthBar);
        }
    }
}