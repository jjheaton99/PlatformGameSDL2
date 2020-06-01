#include "Tile.h"

/*Tile::Tile(Type type, double xPos, double yPos, int size)
    : GameObject(xPos, yPos), m_type{type}, m_size{size}
{
    switchTileType(type);

    m_srcRect.w = 32;
    m_srcRect.h = 32;

    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    m_collider.setDimensions(m_size, m_size);
}

Tile::Tile(int tileNumber, double xPos, double yPos, int size)
    : GameObject(xPos, yPos), m_size{ size }
{
    switchTileTypeFromInt(tileNumber);

    m_srcRect.w = 32;
    m_srcRect.h = 32;

    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    m_collider.setDimensions(m_size, m_size);
}*/

Tile::Tile(std::shared_ptr<WTexture> tileTexture, Type type, double xPos, double yPos, int size)
    : GameObject(xPos, yPos), m_type{ type }, m_size{ size }
{
    m_srcRect = { 0, 0, 32, 32 };
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

/*void Tile::switchTileType(Type tileType)
{
    switch (tileType)
    {
    case BACKGROUND:
        m_type = BACKGROUND;
        m_tileTexture = TileTextures::background;
        break;
    case SOLID:
        m_type = SOLID;
        m_tileTexture = TileTextures::block;
        break;
    case PLATFORM:
        m_type = PLATFORM;
        m_tileTexture = TileTextures::platform;
        break;
    case LADDER:
        m_type = LADDER;
        m_tileTexture = TileTextures::ladder;
        break;
    default:
        m_type = BACKGROUND;
        m_tileTexture = TileTextures::background;
        break;
    }
}

void Tile::switchTileTypeFromInt(int tileType)
{
    switch (tileType)
    {
    case 0:
        m_type = BACKGROUND;
        m_tileTexture = TileTextures::background;
        break;
    case 1:
        m_type = SOLID;
        m_tileTexture = TileTextures::block;
        break;
    case 2:
        m_type = PLATFORM;
        m_tileTexture = TileTextures::platform;
        break;
    case 3:
        m_type = LADDER;
        m_tileTexture = TileTextures::ladder;
        break;
    default:
        m_type = BACKGROUND;
        m_tileTexture = TileTextures::background;
        break;
    }
}*/

void Tile::cameraDraw(const Camera& camera) const
{
    if (m_collider.collideCheck(camera.getCollider()) && m_tileTexture)
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_tileTexture->draw(m_srcRect, relativeDstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}