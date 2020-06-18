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
        BACK,
        NONE
    };

private:
    MenuButton m_fullscreenOff;
    MenuButton m_fullscreenOn;
    MenuButton m_back;

    SettingsSelection m_currentSelection{ NONE };

    int m_buttonWidth;
    int m_buttonHeight;

    int m_bigButtonWidth;
    int m_bigButtonHeight;

    bool settingsControls(SDL_Event& event);
    bool m_fullscreen{ false };

    void cycleUp();
    void cycleDown();
    bool m_joyStickCentered{ true };

public:
    SSettings();
    ~SSettings();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};