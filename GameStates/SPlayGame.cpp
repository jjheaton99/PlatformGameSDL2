#include "SPlayGame.h"

SPlayGame::SPlayGame(std::string mapFile)
{
    int pathLength{ 20 };
    m_map->loadMap(pathLength);

    double playerxSpawn{ ((1.0 * m_map->getPlayerSpawnChunk().getx()) * Constants::chunkWidth * Constants::tileSize) + 700.0 };
    double playerySpawn{ ((1.0 * m_map->getPlayerSpawnChunk().gety()) * Constants::chunkHeight * Constants::tileSize) + 700.0 };
    m_player->setPos(playerxSpawn, playerySpawn);

    //generate random enemy at each spawn point
    for (const auto& spawnPoint : m_map->getEnemySpawnPoints())
    {
        m_objectManager->newRandomEnemy(spawnPoint.getx(), spawnPoint.gety());
    }

    m_objectManager->newItem(GameObject::ItemType::AXE, false, playerxSpawn, playerySpawn);
    m_objectManager->newItem(GameObject::ItemType::BOOMERANG, false, playerxSpawn + 300, playerySpawn);
    m_objectManager->newItem(GameObject::ItemType::BLACK_HOLE, false, playerxSpawn + 800, playerySpawn);

    for (int i{ 0 }; i < 5; ++i)
    {
        m_objectManager->newItem(GameObject::ItemType::POTION, true,
            playerxSpawn - 620, playerySpawn + 1250);
    }

    m_camera.setPos(0, 0);
    m_camera.setBoundary(m_map->getLevelWidth(), m_map->getLevelHeight());

    Mix_PlayMusic(m_music, -1);
    Mix_VolumeMusic(0);
    //m_objectManager->newEnemy(GameObject::EnemyType::SPIDER, playerxSpawn + 500.0, playerySpawn + 500.0);
}

SPlayGame::~SPlayGame()
{
    Mix_FreeMusic(m_music);
}

void SPlayGame::ladderClimbUp()
{
    if (!m_player->isClimbing() && m_player->collidingWithLadder() && !m_player->isDodging())
    {
        m_player->uncrouch();
        m_player->setVel(0, 0);
        //lining player up with ladder
        //player hit box height is used instead of width because of the dest rect width of the player is equal to the height
        //m_player->setPos(m_player->getLadderxPos() + 0.5 * Constants::tileSize - 50.0, m_player->getPos().gety());
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

void SPlayGame::ladderClimbDown()
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

void SPlayGame::jump()
{
    if (m_player->isCrouched())
    {
        m_player->dropThroughPlatform();
    }

    else if (!(m_player->getMovement() == Player::AIRBORNE) && !m_player->isAttacking())
    {
        if (m_player->getMovement() == Player::WALLSLIDE)
        {
            double wallJumpVel{ 18.0 };
            if (m_player->isFacingLeft())
            {
                m_player->setVel(wallJumpVel, -1.5 * wallJumpVel);
                m_player->faceRight();
            }
            else
            {
                m_player->setVel(-wallJumpVel, -1.5 * wallJumpVel);
                m_player->faceLeft();
            }
            m_player->makeAirborne();
            m_jumpSound.play();
        }
        else if (m_player->isClimbing())
        {
            m_player->makeAirborne();
        }
        else
        {
            double jumpVel{ 20.0 };
            double maxJumpxVel{ 15.0 };

            m_jumpSound.play();
            m_player->jumpHigher();
            m_player->dodgeCancel();
            if (std::abs(m_player->getVel().getx()) < maxJumpxVel)
            {
                m_player->setVel(m_player->getVel().getx(), -jumpVel);
            }
            else if (m_player->isFacingLeft())
            {
                m_player->setVel(-maxJumpxVel, -jumpVel);
            }
            else if (!m_player->isFacingLeft())
            {
                m_player->setVel(maxJumpxVel, -jumpVel);
            }
            m_player->makeAirborne();
        }
    }
}

void SPlayGame::holdLeftDodge()
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

void SPlayGame::holdRightDodge()
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

void SPlayGame::defaultDodge()
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

void SPlayGame::playerControlsHold(SDL_GameController* controller)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };
    const Sint16 currentLeftyState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) };
    const Sint16 currentLeftxState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) };

    if (m_player->isJumpingHigher() && (currentKeyState[SDL_SCANCODE_SPACE] 
        || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)))
    {
        m_player->jumpHigher();
    }
    else
    {
        m_player->stopJumpingHigher();
    }

    if (!(m_player->getMovement() == Player::AIRBORNE) && !m_player->isClimbing() && !(m_player->getMovement() == Player::WALLSLIDE))
    {
        if (currentKeyState[SDL_SCANCODE_S] 
            || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
            //increase deadzone for crouch 
            || currentLeftyState > 2 * m_joystickDeadZone)
        {
            m_player->crouch();
        }
        else if (currentKeyState[SDL_SCANCODE_A] 
            || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
            || currentLeftxState < -m_joystickDeadZone)
        {
            m_player->moveLeft();
        }
        else if (currentKeyState[SDL_SCANCODE_D] 
            || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
            || currentLeftxState > m_joystickDeadZone)
        {
            m_player->moveRight();
        }
        else
        {
            m_player->stop();
        }
    }

    else if (m_player->getMovement() == Player::AIRBORNE || m_player->getMovement() == Player::WALLSLIDE)
    {
        if (currentKeyState[SDL_SCANCODE_A] 
            || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
            || currentLeftxState < -m_joystickDeadZone)
        {
            m_player->floatLeft();
        }
        else if (currentKeyState[SDL_SCANCODE_D] 
            || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
            || currentLeftxState > m_joystickDeadZone)
        {
            m_player->floatRight();
        }
    }

    if (currentKeyState[SDL_SCANCODE_W] 
        || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)
        || currentLeftyState < -m_joystickDeadZone)
    {
        ladderClimbUp();
    }
    else if (currentKeyState[SDL_SCANCODE_S] 
        || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        || currentLeftyState > m_joystickDeadZone)
    {
        ladderClimbDown();
    }
    else if (m_player->isClimbing() && m_player->collidingWithLadder())
    {
        m_player->climbStop();
    }

    if (!currentKeyState[SDL_SCANCODE_S] 
        && !SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        && currentLeftyState < 2 * m_joystickDeadZone)
    {
        m_player->uncrouch();
    }
}

void SPlayGame::meleeAttackController(SDL_GameController* controller, Sint16 currentLeftyState, Sint16 currentLeftxState)
{
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        || currentLeftyState > 2 * m_joystickDeadZone)
    {
        m_player->dodgeCancel();
        m_player->downAttack();
    }
    else if (!m_player->isClimbing())
    {
        m_player->dodgeCancel();
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
            || currentLeftxState < -m_joystickDeadZone)
        {
            m_player->meleeAttackLeft();
        }
        else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
            || currentLeftxState > m_joystickDeadZone)
        {
            m_player->meleeAttackRight();
        }
        else if (m_player->isFacingLeft())
        {
            m_player->meleeAttackLeft();
        }
        else
        {
            m_player->meleeAttackRight();
        }
    }
}

void SPlayGame::rangedAttackController(SDL_GameController* controller, Sint16 currentLeftyState, Sint16 currentLeftxState)
{
    m_player->dodgeCancel();
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        || currentLeftxState < -m_joystickDeadZone)
    {
        m_player->shootRangedLeft();
    }
    else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        || currentLeftxState > m_joystickDeadZone)
    {
        m_player->shootRangedRight();
    }
    else if (m_player->isFacingLeft())
    {
        m_player->shootRangedLeft();
    }
    else
    {
        m_player->shootRangedRight();
    }
}

void SPlayGame::playerControlsPress(SDL_Event& event, SDL_GameController* controller)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };
    const Sint16 currentLeftyState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) };
    const Sint16 currentLeftxState{ SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) };

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_e:
            m_player->interact();
            break;

        case SDLK_SPACE:
            jump();
            break;

        case SDLK_LSHIFT:
            if (!m_player->isDodging() && !m_player->dodgeCooling())
            {
                m_player->attackCancel();
                if (currentKeyState[SDL_SCANCODE_A])
                {
                    holdLeftDodge();
                }

                else if (currentKeyState[SDL_SCANCODE_D])
                {
                    holdRightDodge();
                }

                //default dodge in direction currently facing
                else if (!m_player->isClimbing())
                {
                    defaultDodge();
                }
            }
            break;

        case SDLK_f:
            m_player->drinkHealthPotion();
            break;

        default:
            break;
        }
    }

    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            if (currentKeyState[SDL_SCANCODE_S])
            {
                m_player->dodgeCancel();
                m_player->downAttack();
            }
            else if (!m_player->isClimbing())
            {
                m_player->dodgeCancel();
                if (currentKeyState[SDL_SCANCODE_A])
                {
                    m_player->meleeAttackLeft();
                }
                else if (currentKeyState[SDL_SCANCODE_D])
                {
                    m_player->meleeAttackRight();
                }
                else if (m_player->isFacingLeft())
                {
                    m_player->meleeAttackLeft();
                }
                else
                {
                    m_player->meleeAttackRight();
                }
            }
            break;

        case SDL_BUTTON_RIGHT:
            m_player->dodgeCancel();
            if (currentKeyState[SDL_SCANCODE_A])
            {
                m_player->shootRangedLeft();
            }
            else if (currentKeyState[SDL_SCANCODE_D])
            {
                m_player->shootRangedRight();
            }
            else if (m_player->isFacingLeft())
            {
                m_player->shootRangedLeft();
            }
            else
            {
                m_player->shootRangedRight();
            }
            break;

        default:
            break;
        }
    }

    else if (event.type == SDL_CONTROLLERBUTTONDOWN)
    {
        switch (event.cbutton.button)
        {
        case SDL_CONTROLLER_BUTTON_A:
            jump();
            break;

        case SDL_CONTROLLER_BUTTON_B:
            if (!m_player->isDodging() && !m_player->dodgeCooling())
            {
                m_player->attackCancel();
                if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    || currentLeftxState < -m_joystickDeadZone)
                {
                    holdLeftDodge();
                }

                else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    || currentLeftxState > m_joystickDeadZone)
                {
                    holdRightDodge();
                }

                //default dodge in direction currently facing
                else if (!m_player->isClimbing())
                {
                    defaultDodge();
                }
            }
            break;

        case SDL_CONTROLLER_BUTTON_X:
            if (!m_bumperControllerConfig)
            {
                meleeAttackController(controller, currentLeftyState, currentLeftxState);
            }
            else
            {
                m_player->interact();
            }
            break;

        case SDL_CONTROLLER_BUTTON_Y:
            if (!m_bumperControllerConfig)
            {
                rangedAttackController(controller, currentLeftyState, currentLeftxState);
            }
            else
            {
                m_player->drinkHealthPotion();
            }
            break;

        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            if (m_bumperControllerConfig)
            {
                meleeAttackController(controller, currentLeftyState, currentLeftxState);
            }
            else
            {
                m_player->interact();
            }
            break;

        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            if (m_bumperControllerConfig)
            {
                rangedAttackController(controller, currentLeftyState, currentLeftxState);
            }
            else
            {
                m_player->drinkHealthPotion();
            }
            break;

        default:
            break;
        }
    }
}

GameState::State SPlayGame::handleEvents(SDL_GameController* controller)
{
    playerControlsHold(controller);

    for (SDL_Event& element : m_events)
    {
        if ((element.type == SDL_KEYDOWN && element.key.keysym.sym == SDLK_ESCAPE)
            || (element.type == SDL_CONTROLLERBUTTONDOWN && element.cbutton.button == SDL_CONTROLLER_BUTTON_START))
        {
            m_paused = true;
            m_stepTimer.pause();
            return PAUSED;
        }

        if (element.type == SDL_MOUSEMOTION)
        {
            g_window.setMouseCentre();
        }

        playerControlsPress(element, controller);
    }

    return STATE_NULL;
}

GameState::State SPlayGame::update()
{
    double timeStep = m_stepTimer.getTicks() / 1000.0;

    if (!m_paused)
    {
        m_timeAccumulator += timeStep;

        //int updateCount{ 0 };
        //game logic updates as many times as it can within the time between rendered frames
        while (m_timeAccumulator > Constants::updateStep)
        {
            m_updateTimer.start();

            m_timeAccumulator -= Constants::updateStep;

            if (m_player->isDead())
            {
                return GAME_OVER;
            }
            else if (!m_player->isDying())
            {
                m_player->update(m_map->getMap(), m_camera, m_objectManager->getEnemies());
                m_objectManager->update(m_map->getMap(), m_camera, m_player);
                m_UI.update();
                m_map->update();
            }
            else
            {
                m_player->update(m_map->getMap(), m_camera, m_objectManager->getEnemies());
                Mix_PauseMusic();
            }

            //add extra time in case logic update took too long to prevent slowing
            double updateTime{ m_updateTimer.getTicks() / 1000.0 };
            if (updateTime < Constants::updateStep)
            {
                m_timeAccumulator += updateTime;
            }
            else
            {
                //prevent freezing by exceeding update timestep
                m_timeAccumulator += Constants::updateStep;
            }
           
            m_updateTimer.stop();
        }

        //std::cout << updateCount << '\n';
        //updateCount = 0;

        m_stepTimer.start();

        m_camera.resize();

        /*double frameRate{ averageFPS(timeStep) };
        if (frameRate > 0)
        {
            std::cout << frameRate << '\n';
        }*/

        return STATE_NULL;
    }

    m_paused = false;
    m_stepTimer.resume();

    return STATE_NULL;
}

void SPlayGame::render()
{
    m_map->drawMap(m_camera);
    m_objectManager->cameraDrawItems(m_camera);
    m_objectManager->cameraDrawEnemies(m_camera);
    m_player->cameraDraw(m_camera);
    m_objectManager->cameraDrawProjectiles(m_camera);
    m_UI.draw();
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
