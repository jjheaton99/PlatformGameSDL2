#pragma once

#include "SDL.h"
#include "Player.h"

class InputHandler
{
public:
    enum WindowEvent
    {
        EVENT_NULL,
        FULLSCREEN,
        QUIT
    };

    InputHandler();
    ~InputHandler();

    WindowEvent windowEvent(SDL_Event& event);
    void mainMenuControls(SDL_Event& event);
    void playerControlsKeyHold(Player* player);
    void playerControlsKeyPress(Player* player, SDL_Event& event);
};

// static void playerControlsTopDown(Player* player);