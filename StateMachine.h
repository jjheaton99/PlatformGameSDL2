#pragma once

#include "GameState.h"
#include "SMainMenu.h"
#include "SLevel1.h"

class StateMachine
{
public:
    enum State
    {
        STATE_NULL,
        MAIN_MENU,
        LEVEL_1,
        EXIT
    };

private:
    State m_currentStateID{ STATE_NULL };
    State m_nextState{ STATE_NULL };

    GameState* m_currentState{ nullptr };

public:
    void setNextState(State nextState);
    void changeState();
};

