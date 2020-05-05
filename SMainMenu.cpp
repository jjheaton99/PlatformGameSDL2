#include "SMainMenu.h"

SMainMenu::SMainMenu(int buttonWidth, int buttonHeight)
    : m_play{"Assets/MenuButtons/play.png", "Assets/MenuButtons/playSelected.png", 
    buttonWidth, buttonHeight, (g_screenWidth / 2) - (buttonWidth / 2), ((g_screenHeight * 3) / 4) - 100 },
    m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsSelected.png",
    buttonWidth, buttonHeight, (g_screenWidth / 2) - (buttonWidth / 2), ((g_screenHeight * 3) / 4) },
    m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitSelected.png",
    buttonWidth, buttonHeight, (g_screenWidth / 2) - (buttonWidth / 2), ((g_screenHeight * 3) / 4) + 100 },
    m_buttonWidth{buttonWidth}, m_buttonHeight{buttonHeight}
{}

SMainMenu::~SMainMenu()
{}

bool SMainMenu::mainMenuControls(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            if (m_currentSelection != PLAY)
            {
                m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) - 1);
            }
            else
            {
                m_currentSelection = QUIT;
            }
            return false;
        case SDLK_UP:
            if (m_currentSelection != PLAY)
            {
                m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) - 1);
            }
            else
            {
                m_currentSelection = QUIT;
            }
            return false;
        case SDLK_s:
            if (m_currentSelection != QUIT)
            {
                m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) + 1);
            }
            else
            {
                m_currentSelection = PLAY;
            }
            return false;
        case SDLK_DOWN:
            if (m_currentSelection != QUIT)
            {
                m_currentSelection = static_cast<MenuSelection>(static_cast<int>(m_currentSelection) + 1);
            }
            else
            {
                m_currentSelection = PLAY;
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

    return false;
}

GameState::State SMainMenu::handleEvents()
{
    for (SDL_Event& element : m_events)
    {
        if (mainMenuControls(element))
        {
            switch (m_currentSelection)
            {
            case SMainMenu::PLAY:
                return PLAY_GAME;
                break;
            case SMainMenu::SETTINGS:
                return STATE_NULL;
                break;
            case SMainMenu::QUIT:
                return EXIT;
                break;
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