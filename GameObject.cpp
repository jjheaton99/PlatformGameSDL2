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
    m_texture.draw(m_srcRect, m_dstRect);
}

void GameObject::destroy()
{
    m_texture.free();
}