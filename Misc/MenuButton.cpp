#include "MenuButton.h"

MenuButton::MenuButton(const char* fileName, const char* selectedTextureFileName, int width, int height, int xPos, int yPos)
    : TextureObject(fileName, xPos, yPos)
{
    m_srcRect = { 0, 0, width, height };
    m_selectedTexture.load(selectedTextureFileName);
}

MenuButton::~MenuButton()
{
    destroy();
    m_selectedTexture.destroy();
}

//draws a different texture if button is selected
void MenuButton::draw() const
{
    if (m_isSelected)
    {
        m_selectedTexture.draw(m_srcRect, m_dstRect, 0, nullptr, SDL_FLIP_NONE);
    }

    else
    {
        m_texture.draw(m_srcRect, m_dstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}

void MenuButton::setPos(int xPos, int yPos)
{
    m_dstRect.x = xPos;
    m_dstRect.y = yPos;
}

bool MenuButton::mouseIsOnButton() const
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);

    if (x < m_dstRect.x || x > m_dstRect.x + m_dstRect.w
        || y < m_dstRect.y || y > m_dstRect.y + m_dstRect.h)
    {
        return false;
    }

    return true;
}