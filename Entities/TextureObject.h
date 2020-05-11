#pragma once

#include "SDL.h"
#include "Vector2D.h"
#include "WTexture.h"

//base class for all texture based objects
class TextureObject
{
protected:
    //double position vector used for precision over int
    Vector2D<double> m_position;

    WTexture m_texture;
    SDL_Rect m_srcRect;
    SDL_Rect m_dstRect;

public:
    //fileName is the name of the texture png file to load
    TextureObject(const char* fileName, double xPos = 0, double yPos = 0);
    ~TextureObject();

    virtual void draw();
    virtual void destroy();

    const Vector2D<double>& getPos() { return m_position; }
    virtual void setPos(double xPos, double yPos) { m_position = Vector2D{ xPos, yPos }; }
};

