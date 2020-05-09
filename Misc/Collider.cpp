#include "Collider.h"

Collider::Collider(int width, int height)
{
    m_hitBox.w = width;
    m_hitBox.h = height;
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

bool Collider::collideCheck(Collider& collider)
{
    return (m_hitBox.x < (collider.getHitBox().x + collider.getHitBox().w)
        && m_hitBox.x >(collider.getHitBox().x - m_hitBox.w)
        && m_hitBox.y < (collider.getHitBox().y + collider.getHitBox().h)
        && m_hitBox.y >(collider.getHitBox().y - m_hitBox.h));
}