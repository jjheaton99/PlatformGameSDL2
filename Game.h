#pragma once

#include "SDL.h"
#include "WTexture.h"
#include "StateMachine.h"
#include "Constants.h"
#include "GlobalObjects.h"
#include "WWindow.h"
#include <iostream>
#include <vector>

//the game class manages the initialisation, running and closing of the game
class Game
{
private:
    std::unique_ptr<StateMachine> m_stateMachine{ nullptr };

public:
    Game();
    ~Game();

    bool init();
    void close();
    void playGame();
};
