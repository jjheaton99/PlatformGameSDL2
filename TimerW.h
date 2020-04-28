#pragma once

#include "SDL.h"

class TimerW
{
private:
    Uint32 m_startTicks{ 0 };
    Uint32 m_pausedTicks{ 0 };

    bool m_started{ false };
    bool m_paused{ false };

public:
    TimerW();
    ~TimerW();

    void start();
    void stop();
    void pause();
    void resume();

    Uint32 getTicks();

    bool isStarted() { return m_started; }
    bool isPaused() { return m_started && m_paused; }
};

