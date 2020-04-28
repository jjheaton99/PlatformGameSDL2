#include "GameObject.h"

GameObject::GameObject(const char* fileName, double xPos, double yPos)
    : m_position{ xPos, yPos }
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