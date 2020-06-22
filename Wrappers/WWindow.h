#pragma once

#include "SDL.h"
#include "Constants.h"

//a wrapper for SDL_Window
class WWindow
{
private:
    SDL_Window* m_window{ nullptr };

    bool m_minimised{ false };

public:
    WWindow();
    ~WWindow();

    bool init();
    SDL_Renderer* createRenderer();
    void handleEvent(SDL_Event& event);
    void destroy();

    bool isMinimised() const { return m_minimised; }

    void fullscreenOnOff();
    void setMouseCentre();
};

