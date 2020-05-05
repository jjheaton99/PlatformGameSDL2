#include "TextureW.h"

TextureW::TextureW()
{}

TextureW::~TextureW()
{}

void TextureW::load(const char* fileName)
{
    SDL_Surface* tmpSurface{ IMG_Load(fileName) };
    if (tmpSurface == nullptr)
    {
        std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << '\n';
    }

    m_texture = SDL_CreateTextureFromSurface(g_renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void TextureW::draw(SDL_Rect& srcRect, SDL_Rect& dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(g_renderer, m_texture, &srcRect, &dstRect, angle, centre, flip);
}

void TextureW::free()
{
    SDL_DestroyTexture(m_texture);
}

void TextureW::setColour(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_SetTextureColorMod(m_texture, R, G, B);
}

void TextureW::setBlendingMode(SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(m_texture, mode);
}

void TextureW::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(m_texture, alpha);
}