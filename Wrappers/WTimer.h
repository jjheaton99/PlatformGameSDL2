#pragma once

#include "SDL.h"

class WTimer
{
private:
    Uint32 m_startTicks{ 0 };
    Uint32 m_pausedTicks{ 0 };

    bool m_started{ false };
    bool m_paused{ false };

public:
    WTimer();
    ~WTimer();

    void start();
    void stop();
    void pause();
    void resume();

    Uint32 getTicks() const;

    bool isStarted() const { return m_started; }
    bool isPaused() const { return m_started && m_paused; }
};

