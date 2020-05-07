#pragma once

#include "GameState.h"
#include "MenuButton.h"

class SMainMenu : public GameState
{
public:
    enum MenuSelection
    {
        PLAY,
        SETTINGS,
        QUIT
    };

private:
    MenuButton m_play;
    MenuButton m_settings;
    MenuButton m_quit;

    MenuSelection m_currentSelection{ PLAY };

    int m_buttonWidth;
    int m_buttonHeight;

    bool mainMenuControls(SDL_Event& event);

public:
    SMainMenu();
    ~SMainMenu();

    State handleEvents();
    State update();
    void render();
};

