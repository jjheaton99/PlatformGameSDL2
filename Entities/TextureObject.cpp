#include "TextureObject.h"

TextureObject::TextureObject(std::string fileName, double xPos, double yPos)
    : m_position{xPos, yPos}
{
    m_texture.load(fileName);
}

TextureObject::TextureObject(double xPos, double yPos)
    : m_position{ xPos, yPos }
{}

TextureObject::TextureObject(SDL_Color textColour, std::string text, double xPos, double yPos)
    : m_position{ xPos, yPos }
{
    loadText(text, textColour);
}

TextureObject::~TextureObject()
{}

void TextureObject::draw() const
{
    m_texture.draw(m_srcRect, m_dstRect, m_angle, nullptr, SDL_FLIP_NONE);
}

void TextureObject::destroy()
{
    m_texture.destroy();
}

void TextureObject::loadText(std::string text, SDL_Color colour)
{
    m_texture.loadText(text, colour);
}