#include "StateMachine.h"

StateMachine::StateMachine()
{}

StateMachine::~StateMachine()
{}

void StateMachine::setNextState(GameState::State nextState)
{
    if (nextState != GameState::EXIT)
    {
        m_nextState = nextState;
    }
}

void StateMachine::changeState()
{
    if (m_nextState != GameState::STATE_NULL)
    {
        if (m_nextState != GameState::EXIT)
        {
            delete m_currentState;
        }

        switch (m_nextState)
        {
        case GameState::MAIN_MENU:
            m_currentState = new SMainMenu{};
            break;
        case GameState::PLAY_GAME:
            m_currentState = new SPlayGame{};
            break;
        }

        m_currentStateID = m_nextState;

        m_nextState = GameState::STATE_NULL;
    }
}

GameState::State StateMachine::gameLoop()
{
    setNextState(m_currentState->handleEvents());

    if (m_nextState == GameState::STATE_NULL)
    {
        setNextState(m_currentState->update());
    }

    changeState();

    m_currentState->render();
    
    return m_currentStateID;
}