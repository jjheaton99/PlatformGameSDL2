#include "SPaused.h"

SPaused::SPaused()
    : m_resume{ "Assets/MenuButtons/resume.png", "Assets/MenuButtons/resumeS.png", 60, 14 },
    m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsS.png", 60, 14 },
    m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 }
{
    m_buttonWidth = 240;
    m_buttonHeight = 56;

    m_resume.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100,
        m_buttonWidth, m_buttonHeight);
    m_settings.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4),
        m_buttonWidth, m_buttonHeight);
    m_quit.setDstRect((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100,
        m_buttonWidth, m_buttonHeight);

    m_pausedTexture.setSrcRect(0, 0, 40, 8);
    m_pausedTexture.setDstRect((g_screenWidth / 2) - 300, ((g_screenHeight * 3) / 4) - 300, 600, 120);
}

SPaused::~SPaused()
{}

bool SPaused::pausedControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (m_currentSelection == NONE && (event.key.keysym.sym == SDLK_w
            || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_UP
            || event.key.keysym.sym == SDLK_DOWN))
        {
            m_currentSelection = RESUME;
        }

        else
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                if (m_currentSelection != RESUME)
                {
                    m_currentSelection = static_cast<PausedSelection>(static_cast<int>(m_currentSelection) - 1);
                }
                else
                {
                    m_currentSelection = QUIT;
                }
                return false;
            case SDLK_UP:
                if (m_currentSelection != RESUME)
                {
                    m_currentSelection = static_cast<PausedSelection>(static_cast<int>(m_currentSelection) - 1);
                }
                else
                {
                    m_currentSelection = QUIT;
                }
                return false;
            case SDLK_s:
                if (m_currentSelection != QUIT)
                {
                    m_currentSelection = static_cast<PausedSelection>(static_cast<int>(m_currentSelection) + 1);
                }
                else
                {
                    m_currentSelection = RESUME;
                }
                return false;
            case SDLK_DOWN:
                if (m_currentSelection != QUIT)
                {
                    m_currentSelection = static_cast<PausedSelection>(static_cast<int>(m_currentSelection) + 1);
                }
                else
                {
                    m_currentSelection = RESUME;
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
        if (m_resume.mouseIsOnButton())
        {
            m_currentSelection = RESUME;
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

    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (m_resume.mouseIsOnButton())
        {
            return true;
        }

        else if (m_settings.mouseIsOnButton())
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

GameState::State SPaused::handleEvents()
{
    for (SDL_Event& element : m_events)
    {
        if (element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
        {
            return PLAY_GAME;
        }

        if (pausedControls(element))
        {
            switch (m_currentSelection)
            {
            case SPaused::RESUME:
                return PLAY_GAME;
            case SPaused::SETTINGS:
                return GameState::SETTINGS;
            case SPaused::QUIT:
                return MAIN_MENU;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SPaused::update()
{
    m_pausedTexture.setDstRect((g_screenWidth / 2) - 300, ((g_screenHeight * 3) / 4) - 300, 600, 120);
    m_resume.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100);
    m_settings.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4));
    m_quit.setPos((g_screenWidth / 2) - (m_buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100);

    switch (m_currentSelection)
    {
    case SPaused::RESUME:
        m_resume.select();
        m_settings.deselect();
        m_quit.deselect();
        break;
    case SPaused::SETTINGS:
        m_resume.deselect();
        m_settings.select();
        m_quit.deselect();
        break;
    case SPaused::QUIT:
        m_resume.deselect();
        m_settings.deselect();
        m_quit.select();
        break;
    case NONE:
    default:
        break;
    }

    return STATE_NULL;
}

void SPaused::render()
{
    m_pausedTexture.draw();
    m_resume.draw();
    m_settings.draw();
    m_quit.draw();
}