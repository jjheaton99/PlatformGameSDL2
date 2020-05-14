#pragma once

#include "TextureObject.h"

//class for menu control buttons
class MenuButton : public TextureObject
{
private:
    bool m_isSelected{ false };
    //extra texture for when button is selected
    WTexture m_selectedTexture;

public:
    MenuButton(const char* fileName, const char* selectedTextureFileName, int width = 0, int height = 0, int xPos = 0, int yPos = 0);
    ~MenuButton();

    void draw() const override;

    void select() { m_isSelected = true; }
    void deselect() { m_isSelected = false; }

    void setPos(int xPos, int yPos);
    void setDstRect(int xPos, int yPos, int width, int height);
    const SDL_Rect& getDstRect() const { return m_dstRect; }

    bool mouseIsOnButton() const;
};

