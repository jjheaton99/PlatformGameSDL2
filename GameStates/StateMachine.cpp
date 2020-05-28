#include "StateMachine.h"

StateMachine::StateMachine()
{}

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
            m_playGame.reset();
            m_gameStarted = false;
        }
        m_currentState.reset(new SMainMenu{});
        SDL_ShowCursor(SDL_ENABLE);
        break;

    case GameState::PLAY_GAME:
        //creates new game if one doesnt exist
        if (!m_playGame)
        {
            m_playGame.reset(new SPlayGame{});
        }
        m_currentState = m_playGame;
        SDL_ShowCursor(SDL_DISABLE);
        m_gameStarted = true;
        break;

    case GameState::PAUSED:
        m_currentState.reset(new SPaused{});
        SDL_ShowCursor(SDL_ENABLE);
        break;

    case GameState::SETTINGS:
        m_currentState.reset(new SSettings{});
        SDL_ShowCursor(SDL_ENABLE);
        break;

    case GameState::GAME_OVER:
        if (m_gameStarted)
        {
            m_playGame.reset();
            m_gameStarted = false;
        }
        m_currentState.reset(new SGameOver{});
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
        //condition enures we dont delete the gamestate
        if (m_nextState != GameState::EXIT && m_currentStateID != GameState::PLAY_GAME)
        {
            m_currentState.reset();
        }

        if (m_nextState == GameState::PREVIOUS)
        {
            m_nextState = m_prevStateID;
        }

        changeStateSwitch(m_nextState);

        m_prevStateID = m_currentStateID;
        m_currentStateID = m_nextState;
        m_nextState = GameState::STATE_NULL;
    }
}

void StateMachine::gameLoop()
{
    //all event handlers return a game state if a change is required, STATE_NULL if not
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

    //updates only if the state is not changing
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
