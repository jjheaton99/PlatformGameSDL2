#pragma once

#include "SDL.h"
#include "SDL_Image.h"

class TextureManager
{
private:
    TextureManager();
    ~TextureManager();

public:
    static SDL_Texture* loadTexture(const char* fileName);

    static void drawTexture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect);

    static SDL_Renderer* m_renderer;
};
