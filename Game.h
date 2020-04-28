#pragma once

#include "SDL.h"
#include "SDL_Image.h"
#include "TextureW.h"
#include "Player.h"
#include "InputHandler.h"
#include <vector>
#include <iostream>

class Game
{
private:
    bool m_isRunning;
    SDL_Window* m_window;

public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    
    void handleEvents();
    void update();
    void render();
    void close();
    
    bool running() { return m_isRunning; }
};

