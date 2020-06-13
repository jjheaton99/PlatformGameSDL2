#pragma once

#include "SDL.h"
#include "SDL_Image.h"
#include "GlobalObjects.h"
#include <iostream>

//Texture wrapper class
class WTexture
{
private:
    SDL_Texture* m_texture;

public:
    WTexture();
    ~WTexture();

    void load(std::string fileName);
    void draw(const SDL_Rect& srcRect, const SDL_Rect& dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip) const;
    void destroy();
    void setColour(Uint8 R, Uint8 G, Uint8 B);
    void setBlendingMode(SDL_BlendMode mode);
    void setAlpha(Uint8 alpha);
    bool textureIsLoaded() const { return m_texture != nullptr; }
};

