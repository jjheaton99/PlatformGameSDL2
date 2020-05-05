#pragma once

#include "SDL.h"
#include "InputHandler.h"
#include "TimerW.h"

class GameState
{
protected:
    std::vector<SDL_Event> m_events;
    InputHandler m_inputHandler{};

    bool m_setFullscreen{ false };

public:
    enum State
    {
        STATE_NULL,
        MAIN_MENU,
        PLAY_GAME,
        EXIT
    };

    GameState() {};
    ~GameState() {};

    virtual State handleEvents() = 0;
    virtual State update() = 0;
    virtual void render() = 0;

    void resetSetFullscreen() { m_setFullscreen = false; }
    bool hasSetFullscreen() { return m_setFullscreen; }
};

