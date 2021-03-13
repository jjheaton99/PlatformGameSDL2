#pragma once

#include "GameState.h"
#include <fstream>
#include <string>
#include <array>

//state for game settings
class SSettings : public GameState
{
public:
    enum SettingsSelection
    {
        FULLSCREEN,
        VSYNC,
        MASTER_DOWN,
        MASTER_UP,
        SFX_DOWN,
        SFX_UP,
        MUSIC_DOWN,
        MUSIC_UP,
        BACK,
        NONE
    };

private:
    TextureObject m_settingsTexture{ "Assets/MiscTextures/settings.png" };

    MenuButton m_fullscreenOff{ "Assets/MenuButtons/off.png", "Assets/MenuButtons/offS.png", 25, 14 };
    MenuButton m_fullscreenOn{ "Assets/MenuButtons/on.png", "Assets/MenuButtons/onS.png", 25, 14 };
    MenuButton m_vSyncOff{ "Assets/MenuButtons/off.png", "Assets/MenuButtons/offS.png", 25, 14 };
    MenuButton m_vSyncOn{ "Assets/MenuButtons/on.png", "Assets/MenuButtons/onS.png", 25, 14 };
    MenuButton m_masterUp{ "Assets/MenuButtons/rightArrow.png", "Assets/MenuButtons/rightArrowS.png", 14, 14 };
    MenuButton m_masterDown{ "Assets/MenuButtons/leftArrow.png", "Assets/MenuButtons/leftArrowS.png", 14, 14 };
    MenuButton m_sfxUp{ "Assets/MenuButtons/rightArrow.png", "Assets/MenuButtons/rightArrowS.png", 14, 14 };
    MenuButton m_sfxDown{ "Assets/MenuButtons/leftArrow.png", "Assets/MenuButtons/leftArrowS.png", 14, 14 };
    MenuButton m_musicUp{ "Assets/MenuButtons/rightArrow.png", "Assets/MenuButtons/rightArrowS.png", 14, 14 };
    MenuButton m_musicDown{ "Assets/MenuButtons/leftArrow.png", "Assets/MenuButtons/leftArrowS.png", 14, 14 };
    MenuButton m_back{ "Assets/MenuButtons/back.png", "Assets/MenuButtons/backS.png", 60, 14 };

    TextureObject m_fullscreenText{ {86, 233, 255}, "Fullscreen ", g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_vSyncText{ {86, 233, 255}, "VSync ", g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_masterVolText{ {86, 233, 255}, "Master volume ", g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_sfxVolText{ {86, 233, 255}, "Effects volume ", g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_musicVolText{ {86, 233, 255}, "Music volume ", g_screenWidth / 2, g_screenHeight / 2 };

    TextureObject m_masterVolNumber{ {86, 233, 255}, std::to_string(Settings::masterVol), g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_sfxVolNumber{ {86, 233, 255}, std::to_string(Settings::sfxVol), g_screenWidth / 2, g_screenHeight / 2 };
    TextureObject m_musicVolNumber{ {86, 233, 255}, std::to_string(Settings::musicVol), g_screenWidth / 2, g_screenHeight / 2 };

    SettingsSelection m_currentSelection{ NONE };

    bool m_lmbHeld{ false };
    int m_heldCount{ 0 };

    int m_buttonWidth;
    int m_buttonHeight;

    int m_smallButtonWidth;
    int m_smallButtonHeight;

    int m_arrowButtonWidth;
    int m_arrowButtonHeight;

    bool settingsControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();

    void deselectAll();

public:
    SSettings();
    ~SSettings();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();

    static void loadSettingsFromFile();
    static void saveSettingsToFile();
};