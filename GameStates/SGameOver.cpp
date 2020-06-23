#include "SGameOver.h"

SGameOver::SGameOver()
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_bigButtonWidth = 384;
    m_bigButtonHeight = 56;

    m_playAgain.setDstRect((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 50,
        m_bigButtonWidth, m_bigButtonHeight);
    m_quit.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 50,
        m_buttonWidth, m_buttonHeight);

    m_gameOverTexture.setSrcRect(0, 0, 26, 18);
    m_gameOverTexture.setDstRect((g_screenWidth / 2) - 300, ((g_screenHeight * 3) / 4) - 450, 600, 120);
}

SGameOver::~SGameOver()
{}

void SGameOver::cycleUp()
{
    if (m_currentSelection != PLAY_AGAIN)
    {
        m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) - 1);
    }
    else
    {
        m_currentSelection = QUIT;
    }
}

void SGameOver::cycleDown()
{
    if (m_currentSelection != QUIT)
    {
        m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) + 1);
    }
    else
    {
        m_currentSelection = PLAY_AGAIN;
    }
}

bool SGameOver::gameOverControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (m_currentSelection == NONE && (event.key.keysym.sym == SDLK_w
            || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_UP
            || event.key.keysym.sym == SDLK_DOWN))
        {
            m_currentSelection = PLAY_AGAIN;
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
        if (m_playAgain.mouseIsOnButton())
        {
            m_currentSelection = PLAY_AGAIN;
        }

        else if (m_quit.mouseIsOnButton())
        {
            m_currentSelection = QUIT;
        }

        return false;
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
        && (m_playAgain.mouseIsOnButton() || m_quit.mouseIsOnButton()))
    {
        return true;
    }

    else if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        if (m_currentSelection == NONE && (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY || event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
            && (std::abs(event.caxis.value) > m_joystickDeadZone))
        {
            m_currentSelection = PLAY_AGAIN;
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
            m_currentSelection = PLAY_AGAIN;
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

GameState::State SGameOver::handleEvents(SDL_GameController* controller)
{
    for (SDL_Event& element : m_events)
    {
        if (element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
        {
            return MAIN_MENU;
        }

        if (gameOverControls(element))
        {
            switch (m_currentSelection)
            {
            case SGameOver::PLAY_AGAIN:
                return PLAY_GAME;
            case SGameOver::QUIT:
                return GameState::QUIT;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SGameOver::update()
{
    m_gameOverTexture.setDstRect((g_screenWidth / 2) - 300, ((g_screenHeight * 3) / 4) - 450, 600, 270);
    m_playAgain.setPos((g_screenWidth / 2) - (m_bigButtonWidth / 2), ((g_screenHeight * 3) / 4) - 50);
    m_quit.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 50);

    switch (m_currentSelection)
    {
    case SGameOver::PLAY_AGAIN:
        m_playAgain.select();
        m_quit.deselect();
        break;
    case SGameOver::QUIT:
        m_playAgain.deselect();
        m_quit.select();
        break;
    default:
        break;
    }

    return STATE_NULL;
}

void SGameOver::render()
{
    m_gameOverTexture.draw();
    m_playAgain.draw();
    m_quit.draw();
}