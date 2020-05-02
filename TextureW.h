#pragma once

#include "SDL.h"
#include "SDL_Image.h"

//Texture wrapper class
class TextureW
{
public:
    TextureW();
    ~TextureW();

    void load(const char* fileName);
    void draw(SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip);
    void free();
    void setColour(Uint8 R, Uint8 G, Uint8 B);
    void setBlendingMode(SDL_BlendMode mode);
    void setAlpha(Uint8 alpha);

    static SDL_Renderer* m_renderer;

private:
    SDL_Texture* m_texture;
};

