#pragma once

#include "GameState.h"
#include "SMainMenu.h"
#include "SPlayGame.h"
#include "SPaused.h"
#include "SSettings.h"
#include "WWindow.h"

class StateMachine
{
private:
    GameState::State m_currentStateID{GameState::MAIN_MENU};
    GameState::State m_prevStateID{ GameState::MAIN_MENU };
    GameState::State m_nextState{ GameState::STATE_NULL };

    GameState* m_currentState{ new SMainMenu{} };
    SPlayGame* m_playGame{ nullptr };
    bool m_gameStarted{ false };

    void changeStateSwitch(GameState::State state);

public:
    StateMachine();
    ~StateMachine();

    void setNextState(GameState::State nextState);
    void changeState();
    void gameLoop();
    GameState::State getCurrentStateID() const { return m_currentStateID; }
};

