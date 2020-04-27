#pragma once

#include "Vector2D.h"
#include "TextureManager.h"

class GameObject
{
protected:
    Vector2D<double> m_position;

    SDL_Texture* m_objectTexture;
    SDL_Rect m_srcRect, m_destRect;

public:
    GameObject(const char* textureSheet, double x = 0, double y = 0);
    ~GameObject();

    virtual void update() = 0;
    void render();
};
