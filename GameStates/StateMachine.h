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
    GameState::State m_currentStateID;
    GameState::State m_prevStateID;
    GameState::State m_nextState;

    GameState* m_currentState;
    SPlayGame* m_playGame;

public:
    StateMachine(GameState::State currentStateID = GameState::MAIN_MENU, GameState::State nextState = GameState::STATE_NULL);
    ~StateMachine();

    void setNextState(GameState::State nextState);
    void changeState();
    void gameLoop();
    GameState::State getCurrentStateID() { return m_currentStateID; }
};

