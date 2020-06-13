#include "TextureObject.h"

TextureObject::TextureObject(std::string fileName, double xPos, double yPos)
    : m_position{xPos, yPos}
{
    m_texture.load(fileName);
}

TextureObject::TextureObject(double xPos, double yPos)
    : m_position{ xPos, yPos }
{}

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
