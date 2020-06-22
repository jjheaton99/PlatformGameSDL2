#pragma once

#include "GameState.h"

class SQuit : public GameState
{
public:
    enum QuitSelection
    {
        MENU,
        DESKTOP,
        BACK,
        NONE
    };

private:
    MenuButton m_menu{ "Assets/MenuButtons/menu.png", "Assets/MenuButtons/menuS.png", 60, 14 };
    MenuButton m_desktop{ "Assets/MenuButtons/desktop.png", "Assets/MenuButtons/desktopS.png", 60, 14 };
    MenuButton m_back{ "Assets/MenuButtons/back.png", "Assets/MenuButtons/backS.png", 60, 14 };

    QuitSelection m_currentSelection{ NONE };

    TextureObject m_quitTexture{ "Assets/MiscTextures/quit.png" };

    int m_buttonWidth;
    int m_buttonHeight;

    bool quitControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();

public:
    SQuit();
    ~SQuit();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};

