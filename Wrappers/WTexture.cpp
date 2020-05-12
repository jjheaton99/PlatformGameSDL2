#include "WTexture.h"

WTexture::WTexture()
{}

WTexture::~WTexture()
{}

void WTexture::load(const char* fileName)
{
    SDL_Surface* tmpSurface{ IMG_Load(fileName) };
    if (tmpSurface == nullptr)
    {
        std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << '\n';
    }

    m_texture = SDL_CreateTextureFromSurface(g_renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void WTexture::draw(const SDL_Rect& srcRect, const SDL_Rect& dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip) const
{
    SDL_RenderCopyEx(g_renderer, m_texture, &srcRect, &dstRect, angle, centre, flip);
}

void WTexture::free()
{
    SDL_DestroyTexture(m_texture);
}

void WTexture::setColour(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_SetTextureColorMod(m_texture, R, G, B);
}

void WTexture::setBlendingMode(SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(m_texture, mode);
}

void WTexture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(m_texture, alpha);
}