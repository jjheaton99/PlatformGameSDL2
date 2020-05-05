#include "SPlayGame.h"

SPlayGame::SPlayGame(const char* mapFile, const char* playerFile)
    : m_map{ new Map{} }, m_player{ new Player{playerFile, 200, 2000} }
{
    if (!m_map->loadMap(mapFile))
    {
        std::cout << "Map not loaded!" << '\n';
    }

    m_camera.setPos(0, 0);
    m_camera.setBoundary(m_map->getLevelWidth(), m_map->getLevelHeight());
}

SPlayGame::~SPlayGame()
{
    delete m_player;
    delete m_map;
}

GameState::State SPlayGame::handleEvents()
{
    m_events.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        m_events.push_back(event);
    }

    m_inputHandler.playerControlsKeyHold(m_player);

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

        m_inputHandler.playerControlsKeyPress(m_player, element);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    m_player->update(timeStep, m_map->getMap(), m_camera);

    m_stepTimer.start();

    return STATE_NULL;
}

void SPlayGame::render()
{
    m_map->drawMap(m_camera);
    m_player->cameraDraw(m_camera);
}