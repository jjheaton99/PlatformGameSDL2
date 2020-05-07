#include "StateMachine.h"

StateMachine::StateMachine(GameState::State currentStateID, GameState::State nextState)
    : m_currentStateID{currentStateID}, m_nextState{nextState}
{
    m_playGame = new SPlayGame{};

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

void StateMachine::changeState()
{
    if (m_nextState != GameState::STATE_NULL)
    {
        if (m_nextState != GameState::EXIT && m_currentStateID != GameState::PLAY_GAME)
        {
            delete m_currentState;
            m_currentState = nullptr;
        }

        switch (m_nextState)
        {
        case GameState::MAIN_MENU:
            delete m_playGame;
            m_playGame = new SPlayGame{};
            m_currentState = new SMainMenu{};
            SDL_ShowCursor(SDL_ENABLE);
            break;

        case GameState::PLAY_GAME:
            m_currentState = m_playGame;
            SDL_ShowCursor(SDL_DISABLE);
            break;

        case GameState::PAUSED:
            m_currentState = new SPaused{};
            SDL_ShowCursor(SDL_ENABLE);
            break;

        case GameState::SETTINGS:
            m_currentState = new SSettings{};
            SDL_ShowCursor(SDL_ENABLE);
            break;

        case GameState::PREVIOUS:
            switch (m_prevStateID)
            {
            case GameState::MAIN_MENU:
                delete m_playGame;
                m_playGame = new SPlayGame{};
                m_currentState = new SMainMenu{};
                SDL_ShowCursor(SDL_ENABLE);
                m_currentStateID = GameState::MAIN_MENU;
                break;

            case GameState::PLAY_GAME:
                m_currentState = m_playGame;
                SDL_ShowCursor(SDL_DISABLE);
                m_currentStateID = GameState::PLAY_GAME;
                break;

            case GameState::PAUSED:
                m_currentState = new SPaused{};
                SDL_ShowCursor(SDL_ENABLE);
                m_currentStateID = GameState::PAUSED;
                break;

            case GameState::SETTINGS:
                m_currentState = new SSettings{};
                SDL_ShowCursor(SDL_ENABLE);
                m_currentStateID = GameState::SETTINGS;
                break;

            default:
                break;
            }
            break;

        default:
            break;
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
