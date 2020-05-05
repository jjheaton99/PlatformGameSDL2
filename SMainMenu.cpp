#include "SMainMenu.h"

SMainMenu::SMainMenu(int buttonWidth, int buttonHeight)
    : m_play{"Assets/MenuButtons/play.png", "Assets/MenuButtons/playSelected.png", 
    buttonWidth, buttonHeight, (Constants::screenWidth / 2) - (buttonWidth / 2), ((Constants::screenHeight * 3) / 4) - 100 },
    m_settings{ "Assets/MenuButtons/settings.png", "Assets/MenuButtons/settingsSelected.png",
    buttonWidth, buttonHeight, (Constants::screenWidth / 2) - (buttonWidth / 2), ((Constants::screenHeight * 3) / 4) },
    m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitSelected.png",
    buttonWidth, buttonHeight, (Constants::screenWidth / 2) - (buttonWidth / 2), ((Constants::screenHeight * 3) / 4) + 100 }
{}

SMainMenu::~SMainMenu()
{}

GameState::State SMainMenu::handleEvents()
{
    m_events.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        m_events.push_back(event);
    }

    for (SDL_Event& element : m_events)
    {
        switch (m_inputHandler.windowEvent(element))
        {
        case InputHandler::QUIT:
            return EXIT;
        case InputHandler::FULLSCREEN:
            m_setFullscreen = true;
            break;
        case InputHandler::EVENT_NULL:
        default:
            break;
        }
    }

    return STATE_NULL;
}

GameState::State SMainMenu::update()
{
    return STATE_NULL;
}

void SMainMenu::render()
{
    m_play.select();
    m_play.draw();
    m_settings.draw();
    m_quit.draw();
}