#pragma once

#include "GameState.h"
#include "WTexture.h"
#include "MenuButton.h"

class SPaused : public GameState
{
public:
    enum PausedSelection
    {
        RESUME,
        SETTINGS,
        QUIT
    };

private:
    MenuButton m_resume;
    MenuButton m_settings;
    MenuButton m_quit;

    PausedSelection m_currentSelection{ RESUME };

    WTexture m_pausedTexture;
    SDL_Rect m_pausedSrcRect{ 0, 0, 40, 8 };
    SDL_Rect m_pausedDstRect{ (g_screenWidth / 2) - 300, ((g_screenHeight * 3) / 4) - 300, 600, 120 };

    int m_buttonWidth;
    int m_buttonHeight;

    bool pausedControls(SDL_Event& event);

public:
    SPaused();
    ~SPaused();

    State handleEvents();
    State update();
    void render();
};

