#pragma once

#include "SDL.h"
#include "TextureW.h"
#include "StateMachine.h"
#include <iostream>

class Game
{
private:
    bool m_isRunning;
    SDL_Window* m_window;
    StateMachine* m_stateMachine;

public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    
    void singleLoop();
    void close();
    
    bool running() { return m_isRunning; }
};


