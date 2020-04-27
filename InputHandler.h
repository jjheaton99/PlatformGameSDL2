#pragma once

#include "SDL.h"
#include "Player.h"

class InputHandler
{
private:
    InputHandler();
    ~InputHandler();

public:
    static bool windowEvent(SDL_Event event);
    static void playerControlsKeyHold(Player* player);
    static void playerControlsKeyPress(Player* player, SDL_Event event);
};

// static void playerControlsTopDown(Player* player);