#include "SSettings.h"

SSettings::SSettings()
    : m_fullscreenOff{ "Assets/MenuButtons/fullscreenOff.png", "Assets/MenuButtons/fullscreenOffS.png", 96, 14 },
    m_fullscreenOn{ "Assets/MenuButtons/fullscreenOn.png", "Assets/MenuButtons/fullscreenOnS.png", 96, 14 },
    m_back{ "Assets/MenuButtons/back.png", "Assets/MenuButtons/backS.png", 60, 14 }
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_bigButtonWidth = 384;
    m_bigButtonHeight = 56;

    m_fullscreenOff.setDstRect((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 100,
        m_bigButtonWidth, m_bigButtonHeight);
    m_fullscreenOn.setDstRect((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 100,
        m_bigButtonWidth, m_bigButtonHeight);
    m_back.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4),
        m_buttonWidth, m_buttonHeight);
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

        else if (m_back.mouseIsOnButton())
        {
            m_currentSelection = BACK;
        }

        return false;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
        && (m_fullscreenOff.mouseIsOnButton() || m_back.mouseIsOnButton()))
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
    for (SDL_Event& element : m_events)
    {
        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_B))
        {
            return PREVIOUS;
        }

        if (settingsControls(element))
        {
            switch (m_currentSelection)
            {
            case SSettings::FULLSCREEN:
                g_window.fullscreenOnOff();

                switch (m_fullscreen)
                {
                case true:
                    m_fullscreen = false;
                    break;
                case false:
                    m_fullscreen = true;
                    break;
                }
                return STATE_NULL;
            case SSettings::BACK:
                return PREVIOUS;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SSettings::update()
{
    m_fullscreenOff.setPos((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 100);
    m_fullscreenOn.setPos((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 100);
    m_back.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4));

    switch (m_currentSelection)
    {
    case SSettings::FULLSCREEN:
        m_fullscreenOff.select();
        m_fullscreenOn.select();
        m_back.deselect();
        break;
    case SSettings::BACK:
        m_fullscreenOff.deselect();
        m_fullscreenOn.deselect();
        m_back.select();
        break;
    case NONE:
    default:
        break;
    }

    return STATE_NULL;
}

void SSettings::render()
{
    if (m_fullscreen)
    {
        m_fullscreenOn.draw();
        m_back.draw();
    }
    else
    {
        m_fullscreenOff.draw();
        m_back.draw();
    }
}