#pragma once

#include "SDL.h"
#include "WTexture.h"
#include "StateMachine.h"
#include "Constants.h"
#include "GlobalObjects.h"
#include "WWindow.h"
#include <iostream>
#include <vector>

class Game
{
private:
    //SDL_Window* window{ nullptr };
    StateMachine* m_stateMachine{ nullptr };

public:
    Game();
    ~Game();

    bool init();
    void close();
    void playGame();
};
