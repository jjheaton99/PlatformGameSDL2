#include "SSettings.h"

SSettings::SSettings()
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_smallButtonWidth = 100;
    m_smallButtonHeight = 56;

    m_arrowButtonWidth = 56;
    m_arrowButtonHeight = 56;

    m_settingsTexture.setSrcRect(0, 0, 54, 8);
    m_settingsTexture.setDstRect((g_screenWidth / 2) - 405, 80, 810, 120);

    m_fullscreenOff.setDstRect((g_screenWidth / 2) - 50, 300, m_smallButtonWidth, m_smallButtonHeight);
    m_fullscreenOn.setDstRect((g_screenWidth / 2) - 50, 300, m_smallButtonWidth, m_smallButtonHeight);
    m_vSyncOff.setDstRect((g_screenWidth / 2) - 50, 400, m_smallButtonWidth, m_smallButtonHeight);
    m_vSyncOn.setDstRect((g_screenWidth / 2) - 50, 400, m_smallButtonWidth, m_smallButtonHeight);
    m_masterUp.setDstRect((g_screenWidth / 2) + 50, 500, m_arrowButtonWidth, m_arrowButtonHeight);
    m_masterDown.setDstRect((g_screenWidth / 2) - 106, 500, m_arrowButtonWidth, m_arrowButtonHeight);
    m_sfxUp.setDstRect((g_screenWidth / 2) + 50, 600, m_arrowButtonWidth, m_arrowButtonHeight);
    m_sfxDown.setDstRect((g_screenWidth / 2) - 106, 600, m_arrowButtonWidth, m_arrowButtonHeight);
    m_musicUp.setDstRect((g_screenWidth / 2) + 50, 700, m_arrowButtonWidth, m_arrowButtonHeight);
    m_musicDown.setDstRect((g_screenWidth / 2) - 106, 700, m_arrowButtonWidth, m_arrowButtonHeight);
    m_back.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), 800, m_buttonWidth, m_buttonHeight);
}

SSettings::~SSettings()
{}

void SSettings::cycleUp()
{
    if (m_currentSelection != FULLSCREEN)
    {
        m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) - 1);
    }
    else
    {
        m_currentSelection = BACK;
    }
}

void SSettings::cycleDown()
{
    if (m_currentSelection != BACK)
    {
        m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) + 1);
    }
    else
    {
        m_currentSelection = FULLSCREEN;
    }
}

bool SSettings::settingsControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (m_currentSelection == NONE && (event.key.keysym.sym == SDLK_w
            || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_UP
            || event.key.keysym.sym == SDLK_DOWN))
        {
            m_currentSelection = FULLSCREEN;
        }

        else
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_UP:
                cycleUp();
                return false;

            case SDLK_s:
            case SDLK_DOWN:
                cycleDown();
                return false;

            case SDLK_RETURN:
            case SDLK_SPACE:
                return true;

            default:
                return false;
            }
        }
    }

    else if (event.type == SDL_MOUSEMOTION)
    {
        if (m_fullscreenOff.mouseIsOnButton())
        {
            m_currentSelection = FULLSCREEN;
        }
        else if (m_vSyncOn.mouseIsOnButton())
        {
            m_currentSelection = VSYNC;
        }
        else if (m_masterUp.mouseIsOnButton())
        {
            m_currentSelection = MASTER_UP;
        }
        else if (m_masterDown.mouseIsOnButton())
        {
            m_currentSelection = MASTER_DOWN;
        }
        else if (m_sfxUp.mouseIsOnButton())
        {
            m_currentSelection = SFX_UP;
        }
        else if (m_sfxDown.mouseIsOnButton())
        {
            m_currentSelection = SFX_DOWN;
        }
        else if (m_musicUp.mouseIsOnButton())
        {
            m_currentSelection = MUSIC_UP;
        }
        else if (m_musicDown.mouseIsOnButton())
        {
            m_currentSelection = MUSIC_DOWN;
        }
        else if (m_back.mouseIsOnButton())
        {
            m_currentSelection = BACK;
        }

        return false;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT 
        && (m_fullscreenOff.mouseIsOnButton() || m_vSyncOn.mouseIsOnButton() || m_masterUp.mouseIsOnButton()
        || m_masterDown.mouseIsOnButton() || m_sfxUp.mouseIsOnButton() || m_sfxDown.mouseIsOnButton()
        || m_musicUp.mouseIsOnButton() || m_musicDown.mouseIsOnButton() || m_back.mouseIsOnButton()))
    {
        return true;
    }

    else if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        if (m_currentSelection == NONE && (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY || event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
            && (std::abs(event.caxis.value) > m_joystickDeadZone))
        {
            m_currentSelection = FULLSCREEN;
        }

        else
        {
            if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY || event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
            {
                if (event.caxis.value < -m_joystickDeadZone && m_joyStickCentered)
                {
                    cycleUp();
                    m_joyStickCentered = false;
                }
                else if (event.caxis.value > m_joystickDeadZone && m_joyStickCentered)
                {
                    cycleDown();
                    m_joyStickCentered = false;
                }
                else if (std::abs(event.caxis.value) <= m_joystickDeadZone)
                {
                    m_joyStickCentered = true;
                }
            }
        }
    }

    else if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        if (m_currentSelection == NONE && (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP
            || event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN))
        {
            m_currentSelection = FULLSCREEN;
        }

        else
        {
            switch (event.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                cycleUp();
                return false;

            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                cycleDown();
                return false;

            case SDL_CONTROLLER_BUTTON_A:
                if (m_currentSelection != NONE)
                {
                    return true;
                }

            default:
                return false;
            }
        }
    }

    return false;
}

GameState::State SSettings::handleEvents(SDL_GameController* controller)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

    bool heldCycle{ false };
    if (++m_heldCount >= 10)
    {
        heldCycle = true;
        m_heldCount = 0;
    }

    bool buttonHeld{ m_lmbHeld || currentKeyState[SDL_SCANCODE_SPACE] 
        || currentKeyState[SDL_SCANCODE_RETURN] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) };

    bool heldEvent{ buttonHeld && heldCycle && (m_masterUp.mouseIsOnButton()
        || m_masterDown.mouseIsOnButton() || m_sfxUp.mouseIsOnButton() || m_sfxDown.mouseIsOnButton()
        || m_musicUp.mouseIsOnButton() || m_musicDown.mouseIsOnButton()) };

    bool pressEvent{ false };

    for (SDL_Event& element : m_events)
    {
        if (element.type == SDL_MOUSEBUTTONDOWN && element.button.button == SDL_BUTTON_LEFT && !m_lmbHeld)
        {
            m_lmbHeld = true;
        }
        else if (element.type == SDL_MOUSEBUTTONUP && element.button.button == SDL_BUTTON_LEFT && m_lmbHeld)
        {
            m_lmbHeld = false;
        }

        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_B))
        {
            return PREVIOUS;
        }

        if (settingsControls(element))
        {
            pressEvent = true;
        }
    }

    if (heldEvent || pressEvent)
    {
        switch (m_currentSelection)
        {
        case SSettings::FULLSCREEN:
            if (Settings::fullscreen)
            {
                Settings::fullscreen = false;
            }
            else
            {
                Settings::fullscreen = true;
            }
            g_window.fullscreenOnOff();

            return STATE_NULL;

        case SSettings::VSYNC:
            if (Settings::vSync)
            {
                Settings::vSync = false;
            }
            else
            {
                Settings::vSync = true;
            }
            return STATE_NULL;

        case SSettings::MASTER_UP:
            if (Settings::masterVol < 100)
            {
                ++Settings::masterVol;
                m_masterVolNumber.loadText(std::to_string(Settings::masterVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::MASTER_DOWN:
            if (Settings::masterVol > 0)
            {
                --Settings::masterVol;
                m_masterVolNumber.loadText(std::to_string(Settings::masterVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::SFX_UP:
            if (Settings::sfxVol < 100)
            {
                ++Settings::sfxVol;
                m_sfxVolNumber.loadText(std::to_string(Settings::sfxVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::SFX_DOWN:
            if (Settings::sfxVol > 0)
            {
                --Settings::sfxVol;
                m_sfxVolNumber.loadText(std::to_string(Settings::sfxVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::MUSIC_UP:
            if (Settings::musicVol < 100)
            {
                ++Settings::musicVol;
                m_musicVolNumber.loadText(std::to_string(Settings::musicVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::MUSIC_DOWN:
            if (Settings::musicVol > 0)
            {
                --Settings::musicVol;
                m_musicVolNumber.loadText(std::to_string(Settings::musicVol), { 86, 233, 255 });
            }
            return STATE_NULL;

        case SSettings::BACK:
            saveSettingsToFile();
            return PREVIOUS;

        case SSettings::NONE:
        default:
            break;
        }
    }

    return STATE_NULL;
}

GameState::State SSettings::update()
{
    m_settingsTexture.setDstRect((g_screenWidth / 2) - 405, 80, 810, 120);

    m_fullscreenOff.setPos((g_screenWidth / 2) - 50, 300);
    m_fullscreenOn.setPos((g_screenWidth / 2) - 50, 300);
    m_vSyncOff.setPos((g_screenWidth / 2) - 50, 400);
    m_vSyncOn.setPos((g_screenWidth / 2) - 50, 400);
    m_masterUp.setPos((g_screenWidth / 2) + 50, 500);
    m_masterDown.setPos((g_screenWidth / 2) - 106, 500);
    m_sfxUp.setPos((g_screenWidth / 2) + 50, 600);
    m_sfxDown.setPos((g_screenWidth / 2) - 106, 600);
    m_musicUp.setPos((g_screenWidth / 2) + 50, 700);
    m_musicDown.setPos((g_screenWidth / 2) - 106, 700);
    m_back.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), 800);


    m_fullscreenText.setDstRect((g_screenWidth / 2) - (4 * m_fullscreenText.getTextDimensions().x) - 60, 
        310, 4 * m_fullscreenText.getTextDimensions().x, 4 * m_fullscreenText.getTextDimensions().y);
    m_vSyncText.setDstRect((g_screenWidth / 2) - (4 * m_vSyncText.getTextDimensions().x) - 60,
        410, 4 * m_vSyncText.getTextDimensions().x, 4 * m_vSyncText.getTextDimensions().y);
    m_masterVolText.setDstRect((g_screenWidth / 2) - (4 * m_masterVolText.getTextDimensions().x) - 116,
        510, 4 * m_masterVolText.getTextDimensions().x, 4 * m_masterVolText.getTextDimensions().y);
    m_sfxVolText.setDstRect((g_screenWidth / 2) - (4 * m_sfxVolText.getTextDimensions().x) - 116,
        610, 4 * m_sfxVolText.getTextDimensions().x, 4 * m_sfxVolText.getTextDimensions().y);
    m_musicVolText.setDstRect((g_screenWidth / 2) - (4 * m_musicVolText.getTextDimensions().x) - 116,
        710, 4 * m_musicVolText.getTextDimensions().x, 4 * m_musicVolText.getTextDimensions().y);

    m_masterVolNumber.setDstRect((g_screenWidth / 2) - (2 * m_masterVolNumber.getTextDimensions().x), 
        510, 4 * m_masterVolNumber.getTextDimensions().x, 4 * m_masterVolNumber.getTextDimensions().y);
    m_sfxVolNumber.setDstRect((g_screenWidth / 2) - (2 * m_sfxVolNumber.getTextDimensions().x), 
        610, 4 * m_sfxVolNumber.getTextDimensions().x, 4 * m_sfxVolNumber.getTextDimensions().y);
    m_musicVolNumber.setDstRect((g_screenWidth / 2) - (2 * m_musicVolNumber.getTextDimensions().x), 
        710, 4 * m_musicVolNumber.getTextDimensions().x, 4 * m_musicVolNumber.getTextDimensions().y);

    deselectAll();
    switch (m_currentSelection)
    {
    case SSettings::FULLSCREEN:
        m_fullscreenOn.select();
        m_fullscreenOff.select();
        break;
    case SSettings::VSYNC:
        m_vSyncOn.select();
        m_vSyncOff.select();
        break;
    case SSettings::MASTER_UP:
        m_masterUp.select();
        break;
    case SSettings::MASTER_DOWN:
        m_masterDown.select();
        break;
    case SSettings::SFX_UP:
        m_sfxUp.select();
        break;
    case SSettings::SFX_DOWN:
        m_sfxDown.select();
        break;
    case SSettings::MUSIC_UP:
        m_musicUp.select();
        break;
    case SSettings::MUSIC_DOWN:
        m_musicDown.select();
        break;
    case SSettings::BACK:
        m_back.select();
        break;
    case SSettings::NONE:
    default:
        break;
    }

    return STATE_NULL;
}

void SSettings::render()
{
    m_settingsTexture.draw();

    if (Settings::fullscreen)
    {
        m_fullscreenOn.draw();
    }
    else
    {
        m_fullscreenOff.draw();
    }

    if (Settings::vSync)
    {
        m_vSyncOn.draw();
    }
    else
    {
        m_vSyncOff.draw();
    }

    m_masterUp.draw();
    m_masterDown.draw();
    m_sfxUp.draw();
    m_sfxDown.draw();
    m_musicUp.draw();
    m_musicDown.draw();

    m_fullscreenText.draw();
    m_vSyncText.draw();
    m_masterVolText.draw();
    m_sfxVolText.draw();
    m_musicVolText.draw();

    m_masterVolNumber.draw();
    m_sfxVolNumber.draw();
    m_musicVolNumber.draw();

    m_back.draw();
}

void SSettings::deselectAll()
{
    m_fullscreenOn.deselect();
    m_fullscreenOff.deselect();
    m_vSyncOn.deselect();
    m_vSyncOff.deselect();
    m_masterUp.deselect();
    m_masterDown.deselect();
    m_sfxUp.deselect();
    m_sfxDown.deselect();
    m_musicUp.deselect();
    m_musicDown.deselect();
    m_back.deselect();
}

void SSettings::loadSettingsFromFile()
{
    std::array<int, 5> settings;

    std::ifstream file("SaveFiles/settings.txt");

    if (!file.is_open())
    {
        //default settings if first time game is launched
        Settings::fullscreen = false;
        Settings::vSync = false;
        Settings::masterVol = 100;
        Settings::sfxVol = 100;
        Settings::musicVol = 100;
        saveSettingsToFile();
    }
    else
    {
        std::string line;
        int index{ 0 };
        while (!file.eof() && index < static_cast<int>(settings.size()))
        {
            std::getline(file, line);
            settings.at(index++) = std::stoi(line);
        }

        file.close();

        Settings::fullscreen = settings.at(0);
        Settings::vSync = settings.at(1);
        Settings::masterVol = settings.at(2);
        Settings::sfxVol = settings.at(3);
        Settings::musicVol = settings.at(4);
    }
}

void SSettings::saveSettingsToFile()
{
    std::array<int, 5> settings{
        static_cast<int>(Settings::fullscreen),
        static_cast<int>(Settings::vSync),
        Settings::masterVol,
        Settings::sfxVol,
        Settings::musicVol
    };

    std::filesystem::create_directory("SaveFiles");
    std::ofstream file("SaveFiles/settings.txt", std::ofstream::out);

    if (file.is_open())
    {
        for (const auto& setting : settings)
        {
            file << std::to_string(setting) << '\n';
        }

        file.close();
    }
}