#pragma once

#include "GameState.h"

//separate state for pausing game
class SPaused : public GameState
{
public:
    enum PausedSelection
    {
        RESUME,
        CONTROLS,
        SETTINGS,
        QUIT,
        NONE
    };

private:
    MenuButton m_resume{ "Assets/MenuButtons/resume.png", "Assets/MenuButtons/resumeS.png", 60, 14 };
    MenuButton m_controls{ "Assets/MenuButtons/controls.png", "Assets/MenuButtons/controlsS.png", 60, 14 };
    MenuButton m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsS.png", 60, 14 };
    MenuButton m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 };

    PausedSelection m_currentSelection{ NONE };

    TextureObject m_pausedTexture{ "Assets/MiscTextures/paused.png" };

    int m_buttonWidth;
    int m_buttonHeight;

    bool pausedControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();

public:
    SPaused();
    ~SPaused();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};

