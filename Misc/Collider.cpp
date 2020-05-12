#include "Collider.h"

Collider::Collider(int width, int height, int xPos, int yPos)
{
    m_hitBox.w = width;
    m_hitBox.h = height;
    m_hitBox.x = xPos;
    m_hitBox.y = yPos;
}

Collider::~Collider()
{}

void Collider::setPosition(int x, int y)
{
    m_hitBox.x = x;
    m_hitBox.y = y;
}

void Collider::setDimensions(int w, int h)
{
    m_hitBox.w = w;
    m_hitBox.h = h;
}

bool Collider::collideCheck(const Collider& collider) const
{
    return (m_hitBox.x < (collider.getHitBox().x + collider.getHitBox().w)
        && m_hitBox.x >(collider.getHitBox().x - m_hitBox.w)
        && m_hitBox.y < (collider.getHitBox().y + collider.getHitBox().h)
        && m_hitBox.y >(collider.getHitBox().y - m_hitBox.h));
}

//for calculating x or y overlap between two hitboxes
double Collider::axisBoxOverlap(int pos1, int pos2, int size1, int size2)
{
    if (pos1 < pos2)
    {
        return 1.0 * pos1 + size1 - pos2;
    }
    else if (pos1 > pos2)
    {
        return 1.0 * pos2 + size2 - pos1;
    }
    else if (pos1 = pos2)
    {
        if (size1 > size2)
        {
            return size2;
        }
        else
        {
            return size1;
        }
    }
    else
    {
        return -1.0;
    }
}