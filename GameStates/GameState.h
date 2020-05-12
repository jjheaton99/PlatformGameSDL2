#pragma once

#include "SDL.h"
#include "WTimer.h"
#include "GlobalObjects.h"
#include <vector>

//base class for any state of the game
class GameState
{
protected:
    //events are stored in a vector in case we want to handle them in multiple separate functions
    //this ensures events arent 'deleted' through polling in one function before all handling functions have been called
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
    virtual ~GameState() {};

    virtual State handleEvents() = 0;
    virtual State update() = 0;
    virtual void render() = 0;

    void pushEvent(SDL_Event& event) { m_events.push_back(event); }
    void clearEvents() { m_events.clear(); }
};

