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
                if (m_currentSelection != FULLSCREEN)
                {
                    m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) - 1);
                }
                else
                {
                    m_currentSelection = BACK;
                }
                return false;
            case SDLK_UP:
                if (m_currentSelection != FULLSCREEN)
                {
                    m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) - 1);
                }
                else
                {
                    m_currentSelection = BACK;
                }
                return false;
            case SDLK_s:
                if (m_currentSelection != BACK)
                {
                    m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) + 1);
                }
                else
                {
                    m_currentSelection = FULLSCREEN;
                }
                return false;
            case SDLK_DOWN:
                if (m_currentSelection != BACK)
                {
                    m_currentSelection = static_cast<SettingsSelection>(static_cast<int>(m_currentSelection) + 1);
                }
                else
                {
                    m_currentSelection = FULLSCREEN;
                }
                return false;
            case SDLK_RETURN:
                if (m_currentSelection != NONE)
                {
                    return true;
                }
            case SDLK_SPACE:
                if (m_currentSelection != NONE)
                {
                    return true;
                }
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

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (m_fullscreenOff.mouseIsOnButton())
        {
            return true;
        }

        else if (m_back.mouseIsOnButton())
        {
            return true;
        }
    }

    return false;
}

GameState::State SSettings::handleEvents()
{
    for (SDL_Event& element : m_events)
    {
        if (element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
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