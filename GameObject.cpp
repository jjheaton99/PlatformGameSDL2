#include "GameObject.h"

GameObject::GameObject(const char* textureSheet, double x, double y)
    : m_position{ x, y }
{
    m_objectTexture = TextureManager::loadTexture(textureSheet);
}

GameObject::~GameObject()
{
}

void GameObject::render()
{
    SDL_RenderCopy(TextureManager::m_renderer, m_objectTexture, &m_srcRect, &m_destRect);
}

