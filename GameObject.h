#pragma once

#include "Vector2D.h"
#include "TextureW.h"

class GameObject
{
protected:
    Vector2D<double> m_position;

    TextureW m_texture;
    SDL_Rect m_srcRect, m_dstRect;

public:
    GameObject(const char* fileName, double xPos = 0, double yPos = 0);
    ~GameObject();

    virtual void update(double timeStep) = 0;
    virtual void draw();
    virtual void destroy();
};
