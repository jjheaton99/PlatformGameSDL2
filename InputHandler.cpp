#include "InputHandler.h"

InputHandler::InputHandler()
{}

InputHandler::~InputHandler()
{}

bool InputHandler::windowEvent(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        return false;

    else if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            return false;
            break;
        default:
            break;
        }
    }

    return true;
}

void InputHandler::playerControlsKeyHold(Player* player)
{
    if (!player->isAirborne())
    {
        const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

        if (currentKeyState[SDL_SCANCODE_A])
            player->moveLeft();
        else if (currentKeyState[SDL_SCANCODE_D])
            player->moveRight();
        else
            player->stop();
    }
}

void InputHandler::playerControlsKeyPress(Player* player, SDL_Event& event)
{
    if (!player->isAirborne())
    {
        double jumpVel{ 1000 };

        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            player->setVel(player->getVel().getx(), -jumpVel);
            player->makeAirborne();
            break;
        default:
            break;
        }
    }
}

/*
void InputHandler::playerControlsTopDown(Player* player)
{
    const Uint8* currentKeyState{ SDL_GetKeyboardState(nullptr) };

    int vel{ 5 };

    if (currentKeyState[SDL_SCANCODE_D] && currentKeyState[SDL_SCANCODE_W])
    {
        player->setVel(vel, 0);
        player->setDirection(-45);
    }
    else if (currentKeyState[SDL_SCANCODE_W] && currentKeyState[SDL_SCANCODE_A])
    {
        player->setVel(vel, 0);
        player->setDirection(-135);
    }
    else if (currentKeyState[SDL_SCANCODE_A] && currentKeyState[SDL_SCANCODE_S])
    {
        player->setVel(vel, 0);
        player->setDirection(-225);
    }
    else if (currentKeyState[SDL_SCANCODE_S] && currentKeyState[SDL_SCANCODE_D])
    {
        player->setVel(vel, 0);
        player->setDirection(-315);
    }
    else if (currentKeyState[SDL_SCANCODE_W])
        player->setVel(0, -vel);
    else if (currentKeyState[SDL_SCANCODE_S])
        player->setVel(0, vel);
    else if (currentKeyState[SDL_SCANCODE_A])
        player->setVel(-vel, 0);
    else if (currentKeyState[SDL_SCANCODE_D])
        player->setVel(vel, 0);
    else
        player->setVel(0, 0);
}*/

/*        if (currentKeyState[SDL_SCANCODE_A])
            player->setVel(-vel, player->getVel().gety());
        else if (currentKeyState[SDL_SCANCODE_D])
            player->setVel(vel, player->getVel().gety());
        else
            player->setVel(0, player->getVel().gety());*/