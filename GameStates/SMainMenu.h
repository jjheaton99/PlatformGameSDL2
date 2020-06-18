#pragma once

#include "GameState.h"
#include "MenuButton.h"

//main menu state
class SMainMenu : public GameState
{
public:
    enum MenuSelection
    {
        PLAY,
        SETTINGS,
        QUIT,
        NONE
    };

private:
    MenuButton m_play;
    MenuButton m_settings;
    MenuButton m_quit;

    MenuSelection m_currentSelection{ NONE };

    int m_buttonWidth;
    int m_buttonHeight;

    bool mainMenuControls(SDL_Event& event, SDL_GameController* controller);

    void cycleUp();
    void cycleDown();
    bool m_joyStickCentered{ true };

public:
    SMainMenu();
    ~SMainMenu();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};

