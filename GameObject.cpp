#include "GameObject.h"

GameObject::GameObject(const char* textureSheet, int x, int y)
{
    m_objectTexture = TextureManager::loadTexture(textureSheet);

    m_xpos = x;
    m_ypos = y;
}

GameObject::~GameObject()
{
}

void GameObject::render()
{
    SDL_RenderCopy(TextureManager::m_renderer, m_objectTexture, &m_srcRect, &m_destRect);
}

