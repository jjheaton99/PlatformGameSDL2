#include "WTexture.h"

WTexture::WTexture()
{}

WTexture::~WTexture()
{}

void WTexture::load(std::string fileName)
{
    if (m_texture)
    {
        destroy();
    }

    SDL_Surface* tmpSurface{ IMG_Load(fileName.c_str()) };
    if (tmpSurface == nullptr)
    {
        std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << '\n';
    }

    m_texture = SDL_CreateTextureFromSurface(g_renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    m_text = false;
}

void WTexture::loadText(std::string text, SDL_Color textColour)
{
    if (m_texture)
    {
        destroy();
    }

    SDL_Surface* textSurface{ TTF_RenderText_Solid(g_font, text.c_str(), textColour) };
    if (textSurface == nullptr)
    {
        std::cout << "Unable to load text! SDL_Error: " << SDL_GetError() << '\n';
    }

    m_texture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
    m_textDimensions.x = textSurface->w;
    m_textDimensions.y = textSurface->h;
    SDL_FreeSurface(textSurface);

    m_text = true;
}

void WTexture::draw(const SDL_Rect& srcRect, const SDL_Rect& dstRect, double angle, SDL_Point* centre, SDL_RendererFlip flip) const
{
    if (!m_text)
    {
        SDL_RenderCopyEx(g_renderer, m_texture, &srcRect, &dstRect, angle, centre, flip);
    }
    else
    {
        SDL_RenderCopyEx(g_renderer, m_texture, nullptr, &dstRect, angle, centre, flip);
    }
}

void WTexture::destroy()
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