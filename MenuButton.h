#pragma once

#include "TextureObject.h"

class MenuButton : public TextureObject
{
private:
    bool m_isSelected{ false };
    TextureW m_selectedTexture;

public:
    MenuButton(const char* fileName, const char* selectedTextureFileName, int width, int height, int xPos = 0, int yPos = 0);
    ~MenuButton();

    void draw() override;

    void select() { m_isSelected = true; }
    void deselect() { m_isSelected = false; }

    void setPos(int xPos, int yPos);
    SDL_Rect& getDstRect() { return m_dstRect; }
};

