#pragma once

#include "SDL.h"
#include "Player.h"

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();

    bool windowEvent(SDL_Event& event);
    void playerControlsKeyHold(Player* player);
    void playerControlsKeyPress(Player* player, SDL_Event& event);
};

// static void playerControlsTopDown(Player* player);