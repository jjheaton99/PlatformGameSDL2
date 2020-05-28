#include "SPlayGame.h"

SPlayGame::SPlayGame(const char* mapFile)
{
    if (!m_map->loadMap(mapFile))
    {
        std::cout << "Map not loaded!" << '\n';
    }

    m_camera.setPos(0, 0);
    m_camera.setBoundary(m_map->getLevelWidth(), m_map->getLevelHeight());

    for (int i{ 2 }; i < m_camera.getxBoundary() /100.0; ++i)
    {
        m_objectManager->newEnemy(GameObjectManager::Enemy::BAT, 100.0 * i, 2000);
    }
    m_objectManager->newEnemy(GameObjectManager::Enemy::SLIME, 200, 2000);
    m_objectManager->newEnemy(GameObjectManager::Enemy::BAT, 500, 2200);
}

SPlayGame::~SPlayGame()
{}

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
        if (!m_player->isClimbing() && m_player->collidingWithLadder() && !m_player->isDodging())
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
        if (m_player->isClimbing())
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
            if (!(m_player->getMovement() == Player::AIRBORNE))
            {
                double jumpVel{ 25.0 };

                if (m_player->isClimbing())
                {
                    m_player->makeAirborne();
                    break;
                }

                m_player->dodgeCancel();
                m_player->attackCancel();
                if (std::abs(m_player->getVel().getx()) < 15.0)
                {
                    m_player->setVel(m_player->getVel().getx(), -jumpVel);
                }
                else if (m_player->isFacingLeft())
                {
                    m_player->setVel(-15.0, -jumpVel);
                }
                else if (!m_player->isFacingLeft())
                {
                    m_player->setVel(15.0, -jumpVel);
                }
                m_player->makeAirborne();
            }
            break;

        case SDLK_LSHIFT:
            if (!m_player->isDodging() && !m_player->dodgeCooling())
            {
                m_player->attackCancel();
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
                        m_player->addVel(-40.0, 0);
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
                        m_player->addVel(40.0, 0);
                    }

                    m_player->dodgeRight();
                }

                else if (!m_player->isClimbing())
                {
                    if (m_player->isFacingLeft())
                    {
                        if (m_player->getMovement() == Player::AIRBORNE)
                        {
                                m_player->addVel(-12.5, 0);
                        }
                        else
                        {
                            m_player->addVel(-40.0, 0);
                        }

                        m_player->dodgeLeft();
                    }

                    else if (!m_player->isFacingLeft())
                    {
                        if (m_player->getMovement() == Player::AIRBORNE)
                        {
                            m_player->addVel(12.5, 0);
                        }
                        else
                        {
                            m_player->addVel(40.0, 0);
                        }

                        m_player->dodgeRight();
                    }
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
            if (!m_player->isAttacking() && !m_player->isClimbing())
            {
                m_player->dodgeCancel();
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

        if (element.type == SDL_MOUSEMOTION)
        {
            g_window.setMouseCentre();
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
            m_player->update(m_map->getMap(), m_camera, m_objectManager->getEnemies());
            m_objectManager->update(m_map->getMap(), m_camera, *m_player);
            if (m_player->isDead())
            {
                return GAME_OVER;
            }
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
    m_objectManager->cameraDraw(m_camera);
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
