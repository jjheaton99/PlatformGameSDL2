#include "SGameOver.h"

SGameOver::SGameOver()
    : m_playAgain{ "Assets/MenuButtons/playAgain.png", "Assets/MenuButtons/playAgainS.png", 96, 14 },
    m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 }
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
{
    m_gameOverTexture.destroy();
}

bool SGameOver::gameOverControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            if (m_currentSelection != PLAY_AGAIN)
            {
                m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) - 1);
            }
            else
            {
                m_currentSelection = QUIT;
            }
            return false;
        case SDLK_UP:
            if (m_currentSelection != PLAY_AGAIN)
            {
                m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) - 1);
            }
            else
            {
                m_currentSelection = QUIT;
            }
            return false;
        case SDLK_s:
            if (m_currentSelection != QUIT)
            {
                m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) + 1);
            }
            else
            {
                m_currentSelection = PLAY_AGAIN;
            }
            return false;
        case SDLK_DOWN:
            if (m_currentSelection != QUIT)
            {
                m_currentSelection = static_cast<GameOverSelection>(static_cast<int>(m_currentSelection) + 1);
            }
            else
            {
                m_currentSelection = PLAY_AGAIN;
            }
            return false;
        case SDLK_RETURN:
            return true;
        case SDLK_SPACE:
            return true;
        default:
            return false;
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

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (m_playAgain.mouseIsOnButton())
        {
            return true;
        }

        else if (m_quit.mouseIsOnButton())
        {
            return true;
        }

        else if (m_quit.mouseIsOnButton())
        {
            return true;
        }
    }

    return false;
}

GameState::State SGameOver::handleEvents()
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
                return MAIN_MENU;
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