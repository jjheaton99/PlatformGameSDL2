#pragma once

#include "SDL.h"
#include "SDL_Image.h"
#include "GlobalObjects.h"
#include <iostream>

//Texture wrapper class
class WTexture
{
public:
    WTexture();
    ~WTexture();

    void load(const char* fileName);
    void draw(const SDL_Rect& srcRect, const SDL_Rect& dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip) const;
    void free();
    void setColour(Uint8 R, Uint8 G, Uint8 B);
    void setBlendingMode(SDL_BlendMode mode);
    void setAlpha(Uint8 alpha);

    //static SDL_Renderer* renderer;

private:
    SDL_Texture* m_texture;
};

