#include "StateMachine.h"

StateMachine::StateMachine(GameState::State currentStateID, GameState::State nextState)
    : m_currentStateID{currentStateID}, m_nextState{nextState}
{
    switch (m_currentStateID)
    {
    case GameState::MAIN_MENU:
        m_currentState = new SMainMenu{};
        break;
    case GameState::PLAY_GAME:
        m_currentState = new SPlayGame{};
        break;
    default:
        break;
    }
}

StateMachine::~StateMachine()
{}

void StateMachine::setNextState(GameState::State nextState)
{
    m_nextState = nextState;
}

void StateMachine::changeState()
{
    if (m_nextState != GameState::STATE_NULL)
    {
        if (m_nextState != GameState::EXIT)
        {
            delete m_currentState;
            m_currentState = nullptr;
        }

        switch (m_nextState)
        {
        case GameState::MAIN_MENU:
            m_currentState = new SMainMenu{};
            break;
        case GameState::PLAY_GAME:
            m_currentState = new SPlayGame{};
            break;
        default:
            break;
        }

        m_currentStateID = m_nextState;

        m_nextState = GameState::STATE_NULL;
    }
}

void StateMachine::gameLoop()
{
    setNextState(m_currentState->handleEvents());

    if (m_nextState == GameState::STATE_NULL)
    {
        setNextState(m_currentState->update());
    }

    changeState();

    SDL_RenderClear(TextureW::m_renderer);
    m_currentState->render();
    SDL_RenderPresent(TextureW::m_renderer);
}