#include "TextureManager.h"

TextureManager::TextureManager()
{}

TextureManager::~TextureManager()
{}

SDL_Texture* TextureManager::loadTexture(const char* fileName)
{
    SDL_Surface* tmpSurface{ IMG_Load(fileName) };
    SDL_Texture* tex{ SDL_CreateTextureFromSurface(m_renderer, tmpSurface) };
    SDL_FreeSurface(tmpSurface);

    return tex;
}

void TextureManager::drawTexture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect)
{
    SDL_RenderCopy(m_renderer, texture, &srcRect, &destRect);
}