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
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

    if (!(m_player->getMovement() == Player::AIRBORNE) && !m_player->isClimbing())
    {
        if (currentKeyState[SDL_SCANCODE_S])
        {
            m_player->crouch();
        }
        else if (currentKeyState[SDL_SCANCODE_A])
        {
            m_player->moveLeft();
        }
        else if (currentKeyState[SDL_SCANCODE_D])
        {
            m_player->moveRight();
        }
        else
        {
            m_player->stop();
        }
    }

    if (currentKeyState[SDL_SCANCODE_W])
    {
        if (!m_player->isClimbing() && m_player->collidingWithLadder())
        {
            m_player->uncrouch();
            m_player->setVel(0, 0);
            //lining player up with ladder
            //player hit box height is used instead of width because of the dest rect width of the player is equal to the height
            m_player->setPos(m_player->getLadderxPos() + 0.5 * Constants::tileSize - 50.0, m_player->getPos().gety());
            m_player->climbUp();
        }
        else if (m_player->isClimbing())
        {
            if (m_player->collidingWithLadder())
            {
                m_player->climbUp();
            }
            else
            {
                m_player->makeAirborne();
            }
        }
    }
    else if (currentKeyState[SDL_SCANCODE_S])
    {
        if (!m_player->isClimbing() && m_player->collidingWithLadder())
        {
            m_player->uncrouch();
            m_player->setVel(0, 0);
            m_player->setPos(m_player->getLadderxPos() + 0.5 * Constants::tileSize - 50.0, m_player->getPos().gety());
            m_player->climbDown();
        }
        else if (m_player->isClimbing())
        {
            if (m_player->collidingWithLadder())
            {
                m_player->climbDown();
            }
            else
            {
                m_player->makeAirborne();
            }
        }
    }
    else if (m_player->isClimbing() && m_player->collidingWithLadder())
    {
        m_player->climbStop();
    }

    if (!currentKeyState[SDL_SCANCODE_S])
    {
        m_player->uncrouch();
    }
}

void SPlayGame::playerControlsKeyPress(SDL_Event& event)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            if (!(m_player->getMovement() == Player::AIRBORNE) && !m_player->isDodging())
            {
                double jumpVel{ 25.0 };

                if (m_player->isClimbing())
                {
                    m_player->makeAirborne();
                    break;
                }
                m_player->setVel(m_player->getVel().getx(), -jumpVel);
                m_player->makeAirborne();
            }
            break;

        case SDLK_LSHIFT:
            if (!m_player->isDodging() && !m_player->dodgeCooling())
            {
                if (currentKeyState[SDL_SCANCODE_A])
                {
                    if (m_player->isClimbing())
                    {
                        m_player->makeAirborne();
                        m_player->addVel(-12.5, 0);

                    }
                    else if (m_player->getMovement() == Player::AIRBORNE)
                    {
                        if (m_player->getVel().getx() >= 0.0)
                        {
                            m_player->addVel(-12.5, 0);
                        }
                        else if (m_player->getVel().getx() < 0.0)
                        {
                            m_player->addVel(-6.5, 0);
                        }
                    }
                    else
                    {
                        m_player->addVel(-20.0, 0);
                    }

                    m_player->dodgeLeft();
                }

                else if (currentKeyState[SDL_SCANCODE_D])
                {
                    if (m_player->isClimbing())
                    {
                        m_player->makeAirborne();
                        m_player->addVel(12.5, 0);
                    }
                    else if (m_player->getMovement() == Player::AIRBORNE)
                    {
                        if (m_player->getVel().getx() >= 0.0)
                        {
                            m_player->addVel(6.5, 0);
                        }
                        else if (m_player->getVel().getx() < 0.0)
                        {
                            m_player->addVel(12.5, 0);
                        }
                    }
                    else
                    {
                        m_player->addVel(20.0, 0);
                    }

                    m_player->dodgeRight();
                }
            }
            break;

        default:
            break;
        }
    }
}

void SPlayGame::playerControlsMouseClick(SDL_Event& event)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            if (currentKeyState[SDL_SCANCODE_A])
            {
                m_player->attackLeft();
            }
            else if (currentKeyState[SDL_SCANCODE_D])
            {
                m_player->attackRight();
            }
            else if (m_player->isFacingLeft())
            {
                m_player->attackLeft();
            }
            else
            {
                m_player->attackRight();
            }
            break;
        default:
            break;
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
        playerControlsMouseClick(element);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    if (!m_paused)
    {
        m_timeAccumulator += timeStep;

        while (m_timeAccumulator > Constants::updateStep)
        {
            m_timeAccumulator -= Constants::updateStep;
            m_player->update(m_map->getMap(), m_camera);
        }

        m_stepTimer.start();

        m_camera.resize();

        double frameRate{ averageFPS(timeStep) };
        if (frameRate > 0)
        {
            //std::cout << frameRate << '\n';
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
