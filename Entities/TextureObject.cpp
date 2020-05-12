#include "TextureObject.h"

TextureObject::TextureObject(const char* fileName, double xPos, double yPos)
    : m_position{xPos, yPos}
{
    m_texture.load(fileName);
}

TextureObject::~TextureObject()
{}

void TextureObject::draw() const
{
    m_texture.draw(m_srcRect, m_dstRect, 0, nullptr, SDL_FLIP_NONE);
}

void TextureObject::destroy()
{
    m_texture.free();
}
