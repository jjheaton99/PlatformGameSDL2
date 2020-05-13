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
    default:
        break;
    }

    m_prevStateID = m_currentStateID;
}

StateMachine::~StateMachine()
{}

void StateMachine::setNextState(GameState::State nextState)
{
    m_nextState = nextState;
}

void StateMachine::changeStateSwitch(GameState::State state)
{
    switch (state)
    {
    case GameState::MAIN_MENU:
        //if we return to main menu after starting the game, the game is reset
        if (m_gameStarted)
        {
            delete m_playGame;
            m_playGame = nullptr;
            m_gameStarted = false;
        }
        m_currentState = new SMainMenu{};
        SDL_ShowCursor(SDL_ENABLE);
        break;

    case GameState::PLAY_GAME:
        if (!m_playGame)
        {
            m_playGame = new SPlayGame{};
        }
        m_currentState = m_playGame;
        SDL_ShowCursor(SDL_DISABLE);
        m_gameStarted = true;
        break;

    case GameState::PAUSED:
        m_currentState = new SPaused{};
        SDL_ShowCursor(SDL_ENABLE);
        break;

    case GameState::SETTINGS:
        m_currentState = new SSettings{};
        SDL_ShowCursor(SDL_ENABLE);
        break;

    default:
        break;
    }
}

void StateMachine::changeState()
{
    if (m_nextState != GameState::STATE_NULL)
    {
        //condition enures we dont delete nullptr when exiting and dont delete the gamestate
        if (m_nextState != GameState::EXIT && m_currentStateID != GameState::PLAY_GAME)
        {
            delete m_currentState;
            m_currentState = nullptr;
        }

        changeStateSwitch(m_nextState);

        if (m_nextState == GameState::PREVIOUS)
        {
            changeStateSwitch(m_prevStateID);
        }

        m_prevStateID = m_currentStateID;
        if (m_nextState != GameState::PREVIOUS)
        {
            m_currentStateID = m_nextState;
        }
        m_nextState = GameState::STATE_NULL;
    }
}

void StateMachine::gameLoop()
{
    m_currentState->clearEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            setNextState(GameState::EXIT);
            changeState();
            return;
        }

        g_window.handleEvent(event);

        m_currentState->pushEvent(event);
    }

    setNextState(m_currentState->handleEvents());

    if (m_nextState == GameState::STATE_NULL)
    {
        setNextState(m_currentState->update());
    }

    changeState();

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
    SDL_RenderClear(g_renderer);
    m_currentState->render();
    SDL_RenderPresent(g_renderer);
}
