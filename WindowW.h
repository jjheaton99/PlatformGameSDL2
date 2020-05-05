#pragma once

#include "SDL.h"
#include "Constants.h"

class WindowW
{
private:
    SDL_Window* m_window{ nullptr };

    bool m_fullscreen{ false };
    bool m_minimised{ false };

public:
    WindowW();
    ~WindowW();

    bool init();
    SDL_Renderer* createRenderer();
    void handleEvent(SDL_Event& event);
    void destroy();

    bool isFullscreen() { return m_fullscreen; }
    bool isMinimised() { return m_minimised; }
};

