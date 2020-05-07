#pragma once

#include "GameState.h"
#include "WTexture.h"
#include "MenuButton.h"

class SPaused : public GameState
{
private:
    WTexture m_pausedTexture;
    SDL_Rect m_pausedSrcRect{ 0, 0, 40, 8 };
    SDL_Rect m_pausedDstRect{ (g_screenWidth / 2) - 300, 150, 600, 120 };

public:
    SPaused();
    ~SPaused();

    State handleEvents();
    State update();
    void render();
};

