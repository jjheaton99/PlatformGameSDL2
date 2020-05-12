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

void Tile::setPos(double xPos, double yPos)
{
    m_dstRect.x = static_cast<int>(xPos);
    m_dstRect.y = static_cast<int>(yPos);

    m_collider.setPosition(static_cast<int>(xPos), static_cast<int>(yPos));
}