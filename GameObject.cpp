#include "GameObject.h"

GameObject::GameObject(const char* fileName, float xPos, float yPos, int colliderWidth, int colliderHeight)
    : m_position{ xPos, yPos }, m_collider{colliderWidth, colliderHeight}
{
    m_texture.load(fileName);
}

GameObject::~GameObject()
{
}

void GameObject::draw()
{
    m_texture.draw(m_srcRect, m_dstRect, 0, nullptr, SDL_FLIP_NONE);
}

void GameObject::destroy()
{
    m_texture.free();
}