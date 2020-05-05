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

void SPlayGame::playerControlsKeyHold()
{
    if (!(m_player->getMovement() == Player::AIRBORNE))
    {
        const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

        if (currentKeyState[SDL_SCANCODE_A])
            m_player->moveLeft();
        else if (currentKeyState[SDL_SCANCODE_D])
            m_player->moveRight();
        else
            m_player->stop();
    }
}

void SPlayGame::playerControlsKeyPress(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (!(m_player->getMovement() == Player::AIRBORNE))
        {
            double jumpVel{ 1500.0 };

            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                m_player->setVel(m_player->getVel().getx(), -jumpVel);
                m_player->makeAirborne();
                break;
            default:
                break;
            }
        }
    }
}

GameState::State SPlayGame::handleEvents()
{
    playerControlsKeyHold();

    for (SDL_Event& element : m_events)
    {
        playerControlsKeyPress(element);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    m_player->update(timeStep, m_map->getMap(), m_camera);

    m_stepTimer.start();

    m_camera.resize();

    return STATE_NULL;
}

void SPlayGame::render()
{
    m_map->drawMap(m_camera);
    m_player->cameraDraw(m_camera);
}