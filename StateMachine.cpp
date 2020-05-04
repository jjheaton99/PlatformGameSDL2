#include "StateMachine.h"

void StateMachine::setNextState(State nextState)
{
    if (nextState != EXIT)
    {
        m_nextState = nextState;
    }
}

void StateMachine::changeState()
{
    if (m_nextState != STATE_NULL)
    {
        if (m_nextState != EXIT)
        {
            delete m_currentState;
        }

        switch (m_nextState)
        {
        case MAIN_MENU:
            m_currentState = new SMainMenu{};
            break;
        case LEVEL_1:
            m_currentState = new SLevel1{};
            break;
        }

        m_currentStateID = m_nextState;

        m_nextState = STATE_NULL;
    }
}