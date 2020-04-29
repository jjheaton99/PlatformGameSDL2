#include "Tile.h"

Tile::Tile(const char* fileName, double xPos, double yPos, Type type)
    : GameObject(fileName, xPos, yPos), m_type{type}
{
    m_dstRect.w = 50;
    m_dstRect.h = 50;

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

void Tile::update(double timeStep)
{}

void Tile::setDrawPos(double xPos, double yPos)
{
    m_dstRect.x = xPos;
    m_dstRect.y = yPos;
}