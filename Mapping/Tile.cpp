#include "Tile.h"

Tile::Tile(std::shared_ptr<WTexture> tileTexture, int srcRectx, int srcRecty, Type type, double xPos, double yPos, int size)
    : GameObject(xPos, yPos), m_type{ type }, m_size{ size }
{
    m_srcRect = { srcRectx, srcRecty, 32, 32 };
    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    m_collider.setDimensions(m_size, m_size);

    m_tileTexture = tileTexture;
}

void Tile::setPos(double xPos, double yPos)
{
    m_dstRect.x = static_cast<int>(xPos);
    m_dstRect.y = static_cast<int>(yPos);

    m_collider.setPosition(xPos, yPos);
}

void Tile::switchTileTypeAndTexture(std::shared_ptr<WTexture> tileTexture, Type tileType)
{
    m_tileTexture = tileTexture;
    switch (tileType)
    {
    case BACKGROUND:
        m_type = BACKGROUND;
        break;
    case SOLID:
        m_type = SOLID;
        break;
    case PLATFORM:
        m_type = PLATFORM;
        break;
    case LADDER:
        m_type = LADDER;
        break;
    }
}

void Tile::cameraDraw(const Camera& camera) const
{
    if (m_collider.collideCheck(camera.getCollider()) && m_tileTexture)
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_tileTexture->draw(m_srcRect, relativeDstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}

int Tile::coordToMapIndex(int coord)
{
    return (coord - (coord % Constants::tileSize)) / Constants::tileSize;
}

int Tile::coordToMapIndex(double coord)
{
    return static_cast<int>(coord / Constants::tileSize);
}