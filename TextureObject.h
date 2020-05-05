#pragma once

#include "SDL.h"
#include "Vector2D.h"
#include "TextureW.h"

class TextureObject
{
protected:
    Vector2D<double> m_position;

    TextureW m_texture;
    SDL_Rect m_srcRect;
    SDL_Rect m_dstRect;

public:
    TextureObject(const char* fileName, double xPos = 0, double yPos = 0);
    ~TextureObject();

    virtual void draw();
    virtual void destroy();
};

