#pragma once

#include "SDL.h"
#include "InputHandler.h"

class GameState
{
public:
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~GameState() {};
};

