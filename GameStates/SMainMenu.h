#pragma once

#include "GameState.h"

//main menu state
class SMainMenu : public GameState
{
public:
    enum MenuSelection
    {
        PLAY,
        CONTROLS,
        SETTINGS,
        QUIT,
        NONE
    };

private:
    MenuButton m_play{ "Assets/MenuButtons/play.png", "Assets/MenuButtons/playS.png", 60, 14 };
    MenuButton m_controls{ "Assets/MenuButtons/controls.png", "Assets/MenuButtons/controlsS.png", 60, 14 };
    MenuButton m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsS.png", 60, 14 };
    MenuButton m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 };

    MenuSelection m_currentSelection{ NONE };

    int m_buttonWidth;
    int m_buttonHeight;

    bool mainMenuControls(SDL_Event& event, SDL_GameController* controller);

    void cycleUp();
    void cycleDown();

public:
    SMainMenu();
    ~SMainMenu();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};

