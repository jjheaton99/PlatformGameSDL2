#include "Character.h"

Character::Character(std::string fileName, double xStartPos, double yStartPos, double xVel, double yVel, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount)
    : GameObject(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight), m_maxHitPoints{ hitPoints }, m_spriteSheetCount{ spriteSheetCount }
{
    m_hitPoints = m_maxHitPoints;

    m_texture.setBlendingMode(SDL_BLENDMODE_BLEND);
}

Character::~Character()
{}

void Character::getCollideTiles(const std::vector<std::vector<Tile>>& map, int characterRow, int characterColumn)
{
    //delete any existing hitboxes 
    m_solidColliders.clear();
    m_platformColliders.clear();
    m_spikeColliders.clear();

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

            case Tile::SPIKES:
                m_spikeColliders.push_back({
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
    if (!m_dead)
    {
        //objects off the screen are not rendered
        if (m_collider.collideCheck(camera.getCollider()))
        {
            //flip texture based on direction character is facing
            SDL_RendererFlip flip{ SDL_FLIP_NONE };
            if (m_facingLeft)
            {
                flip = SDL_FLIP_HORIZONTAL;
            }

            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, flip);

            //for drawing health bar
            //only draw if alive
            if (!m_killed && m_hitPoints < m_maxHitPoints)
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
            //for testing hitbox
            /*SDL_SetRenderDrawColor(g_renderer, 255, 255, 0, 255);
            SDL_Rect rect;
            rect = { static_cast<int>(m_collider.getHitBox().x - camera.getx()), static_cast<int>(m_collider.getHitBox().y - camera.gety()),
                static_cast<int>(m_collider.getHitBox().w), static_cast<int>(m_collider.getHitBox().h) };
            SDL_RenderFillRect(g_renderer, &rect);*/
        }
    }
}

void Character::cycleDamageFlash()
{
    if (m_damageFlashCount >= 0 && m_damageFlashCount <= 5)
    {
        ++m_damageFlashCount;
        if (m_damageFlashCount > 5)
        {
            m_texture.setColour(255, 255, 255);
        }
    }
}

void Character::addHP(int HP)
{ 
    if (m_hitPoints + HP > m_maxHitPoints)
    {
        m_hitPoints = m_maxHitPoints;
    }
    else
    {
        m_hitPoints += HP;
    }
}

void Character::removeHP(int HP)
{
    if (!(m_hitPoints <= 0))
    {
        if (m_hitPoints - HP <= 0)
        {
            m_hitPoints = 0;
            m_deathSound.play();
        }
        else
        {
            m_hitPoints -= HP;
            m_texture.setColour(255, 100, 100);
            m_damageFlashCount = 0;
        }
        m_takeDamageSound.play();
    }
}

void Character::kill()
{
    m_killed = true;
}

void Character::killAnimation()
{
    ++m_killDelayCount;
    m_texture.setAlpha(static_cast<Uint8>((1.0 - static_cast<double>(m_killDelayCount) / 60.0) * 255));
    int newDstRectw{ static_cast<int>(static_cast<double>(m_dstRect.w) * 0.95) };
    int newDstRecth{ static_cast<int>(static_cast<double>(m_dstRect.h) * 0.95) };
    m_dstRect.x += (m_dstRect.w - newDstRectw) / 2;
    m_dstRect.y += (m_dstRect.h - newDstRecth) / 2;
    m_dstRect.w = newDstRectw;
    m_dstRect.h = newDstRecth;
    if (m_killDelayCount > 60)
    {
        m_dead = true;
    }
}

bool Character::immobilisedSweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ Tile::coordToMapIndex(m_collider.getHitBox().x) };
    int characterRow{ Tile::coordToMapIndex(m_collider.getHitBox().y) };

    getCollideTiles(map, characterRow, characterColumn);
    //sort by greatest overlaps
    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const auto& a, const auto& b) {
        return ((std::get<1>(a) > std::get<1>(b) && std::get<1>(a) > std::get<2>(b)) || (std::get<2>(a) > std::get<1>(b) && std::get<2>(a) > std::get<2>(b)));
        }
    );

    bool xCollision{ false };
    bool yCollision{ false };

    Vector2D<double> tempVel{ m_velocity };

    double deflectionFactor{ 1.0 };
    for (auto& sweptCollider : m_solidColliders)
    {
        //if we have had collision in both directions, skip
        if (!xCollision || !yCollision)
        {
            auto result(m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider));

            switch (result.first)
            {
            case Collider::TOP:
            case Collider::BOTTOM:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(-deflectionFactor);
                    yCollision = true;
                }
                break;

            case Collider::LEFT:
            case Collider::RIGHT:
                if (!xCollision)
                {
                    tempVel.xScale(result.second);
                    m_velocity.xScale(-deflectionFactor);
                    xCollision = true;
                }
                break;

            case Collider::NONE:
            default:
                break;
            }
        }
    }

    if (xCollision || yCollision)
    {
        m_position.add(tempVel);
        setCollider();
    }

    return xCollision || yCollision;
}