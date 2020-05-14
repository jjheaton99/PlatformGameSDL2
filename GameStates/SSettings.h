#pragma once

#include "GameState.h"
#include "MenuButton.h"

//state for game settings
class SSettings : public GameState
{
public:
    enum SettingsSelection
    {
        FULLSCREEN,
        BACK
    };

private:
    MenuButton m_fullscreenOff;
    MenuButton m_fullscreenOn;
    MenuButton m_back;

    SettingsSelection m_currentSelection{ BACK };

    int m_buttonWidth;
    int m_buttonHeight;

    int m_bigButtonWidth;
    int m_bigButtonHeight;

    bool settingsControls(SDL_Event& event);
    bool m_fullscreen{ false };

public:
    SSettings();
    ~SSettings();

    State handleEvents();
    State update();
    void render();
};