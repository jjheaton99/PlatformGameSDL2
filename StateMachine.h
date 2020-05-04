#pragma once

#include "GameState.h"
#include "SMainMenu.h"
#include "SPlayGame.h"

class StateMachine
{
private:
    GameState::State m_currentStateID{ GameState::STATE_NULL };
    GameState::State m_nextState{ GameState::STATE_NULL };

    GameState* m_currentState{ nullptr };

public:
    StateMachine();
    ~StateMachine();

    void setNextState(GameState::State nextState);
    void changeState();
    GameState::State gameLoop();

};

