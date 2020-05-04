#include "GameObject.h"

GameObject::GameObject(const char* fileName, double xPos, double yPos, int colliderWidth, int colliderHeight)
    : m_position{ xPos, yPos }, m_collider{colliderWidth, colliderHeight}
{
    m_texture.load(fileName);
}

GameObject::~GameObject()
{
}

void GameObject::draw(Camera& camera)
{
    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}

void GameObject::destroy()
{
    m_texture.free();
}