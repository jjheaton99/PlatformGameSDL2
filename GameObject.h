#pragma once

#include "TextureManager.h"

class GameObject
{
protected:
    int m_xpos;
    int m_ypos;

    SDL_Texture* m_objectTexture;
    SDL_Rect m_srcRect, m_destRect;

public:
    GameObject(const char* textureSheet, int x, int y);
    ~GameObject();

    virtual void update() = 0;
    void render();
};
