#include "SMainMenu.h"

SMainMenu::SMainMenu()
    : m_play{ "Assets/MenuButtons/play.png", "Assets/MenuButtons/playS.png", 60, 14 },
    m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsS.png", 60, 14 },
    m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 }
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_play.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100, 
        m_buttonWidth, m_buttonHeight);
    m_settings.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4),
        m_buttonWidth, m_buttonHeight);
    m_quit.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100,
        m_buttonWidth, m_buttonHeight);
}

SMainMenu::~SMainMenu()
{}

void SMainMenu::cycleUp()
{
    if (m_currentSelection != PLAY)
    {
        m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) - 1);
    }
    else
    {
        m_currentSelection = QUIT;
    }
}

void SMainMenu::cycleDown()
{
    if (m_currentSelection != QUIT)
    {
        m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) + 1);
    }
    else
    {
        m_currentSelection = PLAY;
    }
}

bool SMainMenu::mainMenuControls(SDL_Event& event, SDL_GameController* controller)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (m_currentSelection == NONE && (event.key.keysym.sym == SDLK_w
            || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_UP
            || event.key.keysym.sym == SDLK_DOWN))
        {
            m_currentSelection = PLAY;
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
        if (m_play.mouseIsOnButton())
        {
            m_currentSelection = PLAY;
        }

        else if (m_settings.mouseIsOnButton())
        {
            m_currentSelection = SETTINGS;
        }

        else if (m_quit.mouseIsOnButton())
        {
            m_currentSelection = QUIT;
        }

        return false;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
        && (m_play.mouseIsOnButton() || m_settings.mouseIsOnButton() || m_quit.mouseIsOnButton()))
    {
        return true;
    }

    else if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        if (m_currentSelection == NONE && (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY || event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
            && (std::abs(event.caxis.value) > m_joystickDeadZone))
        {
            m_currentSelection = PLAY;
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
            m_currentSelection = PLAY;
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

GameState::State SMainMenu::handleEvents(SDL_GameController* controller)
{
    for (SDL_Event& element : m_events)
    {
        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_B))
        {
            return EXIT;
        }

        if (mainMenuControls(element, controller))
        {
            switch (m_currentSelection)
            {
            case SMainMenu::PLAY:
                return PLAY_GAME;
            case SMainMenu::SETTINGS:
                return GameState::SETTINGS;
            case SMainMenu::QUIT:
                return EXIT;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SMainMenu::update()
{
    m_play.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100);
    m_settings.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4));
    m_quit.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100);
    
    switch (m_currentSelection)
    {
    case SMainMenu::PLAY:
        m_play.select();
        m_settings.deselect();
        m_quit.deselect();
        break;
    case SMainMenu::SETTINGS:
        m_play.deselect();
        m_settings.select();
        m_quit.deselect();
        break;
    case SMainMenu::QUIT:
        m_play.deselect();
        m_settings.deselect();
        m_quit.select();
        break;
    case NONE:
    default:
        break;
    }

    return STATE_NULL;
}

void SMainMenu::render()
{
    m_play.draw();
    m_settings.draw();
    m_quit.draw();
}