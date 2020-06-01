#pragma once

#include "GameState.h"
#include "SMainMenu.h"
#include "SPlayGame.h"
#include "SPaused.h"
#include "SSettings.h"
#include "SGameOver.h"
#include "WWindow.h"
#include <memory>

//a class to control the flow of game states
class StateMachine
{
private:
    GameState::State m_currentStateID{GameState::MAIN_MENU};
    GameState::State m_prevStateID{ GameState::MAIN_MENU };
    GameState::State m_nextState{ GameState::STATE_NULL };

    std::shared_ptr<GameState> m_currentState{ std::make_shared<SMainMenu>() };
    //play game state kept separate to allow it to persist through certain states eg pause
    std::shared_ptr<SPlayGame> m_playGame{ nullptr };
    bool m_gameStarted{ false };

    TextureObject m_loading{ "Assets/MiscTextures/loading.png" };

    void changeStateSwitch(GameState::State state);
    void drawLoadingScreen();

public:
    StateMachine();
    ~StateMachine();

    void setNextState(GameState::State nextState);
    void changeState();
    void gameLoop();
    GameState::State getCurrentStateID() const { return m_currentStateID; }
};

