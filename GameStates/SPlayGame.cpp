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
        if (element.type == SDL_KEYDOWN)
        {
            switch (element.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_paused = true;
                m_stepTimer.pause();
                return PAUSED;
            case SDLK_F2:
                m_paused = true;
                return MAIN_MENU;
            default:
                break;
            }
        }

        playerControlsKeyPress(element);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    if (!m_paused)
    {
        m_player->update(timeStep, m_map->getMap(), m_camera);

        m_stepTimer.start();

        m_camera.resize();

        double frameRate{ averageFPS(timeStep) };
        if (frameRate > 0)
        {
            std::cout << frameRate << '\n';
        }

        return STATE_NULL;
    }

    m_paused = false;
    m_stepTimer.resume();

    return STATE_NULL;
}

void SPlayGame::render()
{
    m_map->drawMap(m_camera);
    m_player->cameraDraw(m_camera);
}

double SPlayGame::averageFPS(double timeStep)
{
    m_FPSTime += timeStep;
    ++m_FPSCount;
    if (m_FPSTime > 0.1)
    {
        double averageFPS{ m_FPSCount / m_FPSTime };
        m_FPSCount = 0;
        m_FPSTime = 0.0;
        return averageFPS;
    }

    return -1.0;
}
