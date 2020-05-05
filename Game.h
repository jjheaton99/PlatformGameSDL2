#pragma once

#include "SDL.h"
#include "TextureW.h"
#include "StateMachine.h"
#include "Constants.h"
#include <iostream>
#include <vector>

class Game
{
private:
    SDL_Window* window{ nullptr };
    StateMachine* stateMachine{ nullptr };

    bool m_isFullscreen{ false };

public:
    Game();
    ~Game();

    void init();
    void close();
    void gameLoop();
};
