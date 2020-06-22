#include "SQuit.h"

SQuit::SQuit()
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_menu.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100,
        m_buttonWidth, m_buttonHeight);
    m_desktop.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4),
        m_buttonWidth, m_buttonHeight);
    m_back.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100,
        m_buttonWidth, m_buttonHeight);

    m_quitTexture.setSrcRect(0, 0, 26, 8);
    m_quitTexture.setDstRect((g_screenWidth / 2) - 195, ((g_screenHeight * 3) / 4) - 300, 390, 120);
}

SQuit::~SQuit()
{}

void SQuit::cycleUp()
{
    if (m_currentSelection != MENU)
    {
        m_currentSelection = static_cast<QuitSelection>(static_cast<int>(m_currentSelection) - 1);
    }
    else
    {
        m_currentSelection = BACK;
    }
}

void SQuit::cycleDown()
{
    if (m_currentSelection != BACK)
    {
        m_currentSelection = static_cast<QuitSelection>(static_cast<int>(m_currentSelection) + 1);
    }
    else
    {
        m_currentSelection = MENU;
    }
}

bool SQuit::quitControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (m_currentSelection == NONE && (event.key.keysym.sym == SDLK_w
            || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_UP
            || event.key.keysym.sym == SDLK_DOWN))
        {
            m_currentSelection = MENU;
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
        if (m_menu.mouseIsOnButton())
        {
            m_currentSelection = MENU;
        }

        else if (m_desktop.mouseIsOnButton())
        {
            m_currentSelection = DESKTOP;
        }

        else if (m_back.mouseIsOnButton())
        {
            m_currentSelection = BACK;
        }

        return false;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
        && (m_menu.mouseIsOnButton() || m_desktop.mouseIsOnButton() || m_back.mouseIsOnButton()))
    {
        return true;
    }

    else if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        if (m_currentSelection == NONE && (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY || event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
            && (std::abs(event.caxis.value) > m_joystickDeadZone))
        {
            m_currentSelection = MENU;
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
            m_currentSelection = MENU;
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

GameState::State SQuit::handleEvents(SDL_GameController* controller)
{
    for (SDL_Event& element : m_events)
    {
        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_B))
        {
            return PREVIOUS;
        }

        if (quitControls(element))
        {
            switch (m_currentSelection)
            {
            case SQuit::MENU:
                return GameState::MAIN_MENU;
            case SQuit::DESKTOP:
                return GameState::EXIT;
            case SQuit::BACK:
                return GameState::PREVIOUS;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SQuit::update()
{
    m_quitTexture.setDstRect((g_screenWidth / 2) - 195, ((g_screenHeight * 3) / 4) - 300, 390, 120);
    m_menu.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100);
    m_desktop.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4));
    m_back.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100);

    switch (m_currentSelection)
    {
    case SQuit::MENU:
        m_menu.select();
        m_desktop.deselect();
        m_back.deselect();
        break;
    case SQuit::DESKTOP:
        m_menu.deselect();
        m_desktop.select();
        m_back.deselect();
        break;
    case SQuit::BACK:
        m_menu.deselect();
        m_desktop.deselect();
        m_back.select();
        break;
    case NONE:
    default:
        break;
    }

    return STATE_NULL;
}

void SQuit::render()
{
    m_quitTexture.draw();
    m_menu.draw();
    m_desktop.draw();
    m_back.draw();
}