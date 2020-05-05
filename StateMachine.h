#pragma once

#include "GameState.h"
#include "SMainMenu.h"
#include "SPlayGame.h"

class StateMachine
{
private:
    GameState::State m_currentStateID;
    GameState::State m_nextState;

    GameState* m_currentState;

    bool m_setFullscreen{ false };

public:
    StateMachine(GameState::State currentStateID = GameState::MAIN_MENU, GameState::State nextState = GameState::STATE_NULL);
    ~StateMachine();

    void setNextState(GameState::State nextState);
    void changeState();
    void gameLoop();
    GameState::State getCurrentStateID() { return m_currentStateID; }

    void resetSetFullscreen() { m_setFullscreen = false; }
    bool hasSetFullscreen() { return m_setFullscreen; }
};

