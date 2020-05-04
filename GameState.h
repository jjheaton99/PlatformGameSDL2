#pragma once

#include "SDL.h"
#include "InputHandler.h"
#include "TimerW.h"

class GameState
{
public:
    enum State
    {
        STATE_NULL,
        MAIN_MENU,
        PLAY_GAME,
        EXIT
    };

    virtual State handleEvents() = 0;
    virtual State update() = 0;
    virtual void render() = 0;
    virtual ~GameState() {};
};

