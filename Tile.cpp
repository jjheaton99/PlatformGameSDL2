#include "Tile.h"

Tile::Tile(const char* fileName, Type type, double xPos, double yPos, int size)
    : GameObject(fileName, xPos, yPos), m_type{type}, m_size{size}
{
    m_srcRect.w = 32;
    m_srcRect.h = 32;

    m_dstRect.w = m_size;
    m_dstRect.h = m_size;

    switch (m_type)
    {
    case Tile::BACKGROUND:
        m_collider.setDimensions(0, 0);
        break;
    case Tile::SOLID:
        m_collider.setDimensions(50, 50);
        break;
    default:
        break;
    }
}

Tile::~Tile()
{
    destroy();
}

void Tile::setPos(int xPos, int yPos)
{
    m_dstRect.x = xPos;
    m_dstRect.y = yPos;

    m_collider.setPosition(xPos, yPos);
}