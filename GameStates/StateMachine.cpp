#include "StateMachine.h"

StateMachine::StateMachine()
{
    m_loading.setSrcRect(0, 0, 96, 14);
    m_loading.setDstRect(g_screenWidth - 300, g_screenHeight - 70, 384, 56);

    if (SDL_GameControllerAddMappingsFromFile("Assets/gamecontrollerdb.txt") < 0)
    {
        std::cout << "Unable to add mappings from specified file! SDL_Error: " << SDL_GetError() << '\n';
    }
}

StateMachine::~StateMachine()
{
    SDL_GameControllerClose(m_controller);
}

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
            if (m_vSync != Settings::vSync)
            {
                SDL_DestroyRenderer(g_renderer);
                g_renderer = g_window.createRenderer();
                m_vSync = Settings::vSync;
            }
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

    case GameState::QUIT:
        m_currentState.reset(new SQuit{});
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

        drawLoadingScreen();
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
        else if (event.type == SDL_JOYDEVICEADDED)
        {
            for (int i{ 0 }; i < SDL_NumJoysticks(); ++i)
            {
                if (SDL_IsGameController(i))
                {
                    m_controller = SDL_GameControllerOpen(i);
                    if (m_controller)
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Unable to open game controller! SDL_Error: " << SDL_GetError() << '\n';
                    }
                }
            }
        }

        g_window.handleEvent(event);

        m_currentState->pushEvent(event);
    }

    setNextState(m_currentState->handleEvents(m_controller));

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

void StateMachine::drawLoadingScreen()
{
    m_loading.setDstRect(g_screenWidth - 300, g_screenHeight - 70, 384, 56);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);
    m_loading.draw();
    SDL_RenderPresent(g_renderer);
}