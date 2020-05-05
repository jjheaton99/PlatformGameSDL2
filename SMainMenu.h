#pragma once

#include "GameState.h"
#include "MenuButton.h"

class SMainMenu : public GameState
{
public:
    enum MenuState
    {
        PLAY,
        OPTIONS,
        QUIT
    };

private:
    MenuButton m_play;
    MenuButton m_settings;
    MenuButton m_quit;

public:
    SMainMenu(int buttonWidth = 59 * 4, int buttonHeight = 14 * 4);
    ~SMainMenu();

    State handleEvents();
    State update();
    void render();
};

