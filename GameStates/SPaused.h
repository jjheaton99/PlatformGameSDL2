#pragma once

#include "GameState.h"
#include "MenuButton.h"

//separate state for pausing game
class SPaused : public GameState
{
public:
    enum PausedSelection
    {
        RESUME,
        SETTINGS,
        QUIT,
        NONE
    };

private:
    MenuButton m_resume;
    MenuButton m_settings;
    MenuButton m_quit;

    PausedSelection m_currentSelection{ NONE };

    TextureObject m_pausedTexture{ "Assets/MiscTextures/paused.png" };

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

