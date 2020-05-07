#pragma once

#include "SDL.h"
#include "WTimer.h"
#include "GlobalObjects.h"
#include <vector>

class GameState
{
protected:
    std::vector<SDL_Event> m_events;

public:
    enum State
    {
        STATE_NULL,
        MAIN_MENU,
        PLAY_GAME,
        PAUSED,
        SETTINGS,
        PREVIOUS,
        EXIT
    };

    GameState() {};
    ~GameState() {};

    virtual State handleEvents() = 0;
    virtual State update() = 0;
    virtual void render() = 0;

    void pushEvent(SDL_Event& event) { m_events.push_back(event); }
    void clearEvents() { m_events.clear(); }
};

