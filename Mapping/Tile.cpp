#include "Tile.h"

Tile::Tile(const char* fileName, Type type, double xPos, double yPos, int size)
    : GameObject(fileName, xPos, yPos), m_type{type}, m_size{size}
{
    m_srcRect.w = 32;
    m_srcRect.h = 32;

    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    m_collider.setDimensions(m_size, m_size);
}

Tile::Tile(int tileNumber, double xPos, double yPos, int size)
    : GameObject(xPos, yPos), m_size{ size }
{
    switchTileTypeAndTextureFromInt(tileNumber);

    m_srcRect.w = 32;
    m_srcRect.h = 32;

    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    m_collider.setDimensions(m_size, m_size);
}

void Tile::setPos(double xPos, double yPos)
{
    m_dstRect.x = static_cast<int>(xPos);
    m_dstRect.y = static_cast<int>(yPos);

    m_collider.setPosition(xPos, yPos);
}

void Tile::switchTileTypeAndTexture(Type tileType, const char* fileName)
{
    //ensure old tile texture is destroyed to prevent memory leak
    m_texture.load(fileName);

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
    default:
        m_type = BACKGROUND;
        break;
    }
}

void Tile::switchTileTypeAndTextureFromInt(int tileType)
{
    //ensure old tile texture is destroyed to prevent memory leak
    if (m_texture.textureIsLoaded())
    {
        destroy();
    }

    switch (tileType)
    {
    case 0:
        m_texture.load("Assets/MapTiles/blackGrey.png");
        m_type = BACKGROUND;
        break;
    case 1:
        m_texture.load("Assets/MapTiles/WhiteFadeBlocks/1.png");
        m_type = SOLID;
        break;
    case 2:
        m_texture.load("Assets/MapTiles/platform.png");
        m_type = PLATFORM;
        break;
    case 3:
        m_texture.load("Assets/MapTiles/ladder.png.");
        m_type = LADDER;
        break;
    default:
        m_texture.load("Assets/MapTiles/blackGrey.png");
        m_type = BACKGROUND;
        break;
    }
}
