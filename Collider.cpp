#include "Collider.h"

Collider::Collider(int width, int height)
{
    m_box.w = width;
    m_box.h = height;
}

Collider::~Collider()
{}

void Collider::setPosition(int x, int y)
{
    m_box.x = x;
    m_box.y = y;
}

void Collider::setDimensions(int w, int h)
{
    m_box.w = w;
    m_box.h = h;
}

bool Collider::collideCheck(Collider& collider)
{
    return (m_box.x < (collider.getBox().x + collider.getBox().w)
        && m_box.x >(collider.getBox().x - m_box.w)
        && m_box.y < (collider.getBox().y + collider.getBox().h)
        && m_box.y >(collider.getBox().y - m_box.h));
}