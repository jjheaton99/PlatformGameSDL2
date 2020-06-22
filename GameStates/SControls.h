#pragma once

#include "GameState.h"

class SControls : public GameState
{
public:
    enum ControlsSelection
    {
        BACK,
        NONE
    };

private:
    MenuButton m_back{ "Assets/MenuButtons/back.png", "Assets/MenuButtons/backS.png", 60, 14 };

    ControlsSelection m_currentSelection{ NONE };

    TextureObject m_controlsTexture{ "Assets/MiscTextures/controls.png" };

    int m_buttonWidth;
    int m_buttonHeight;

    bool controlsControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();

public:
    SControls();
    ~SControls();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};

