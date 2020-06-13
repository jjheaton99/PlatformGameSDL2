#include "Game.h"

Game::Game()
{}

Game::~Game()
{}

//initialiser function sets up SDL components as well as the state machine 
bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL initialisation unsuccessful! SDL_Error: " << SDL_GetError() << '\n';
        return false;
    }

    else if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "IMG initialisation unsuccessful! IMG_Error: " << IMG_GetError() << '\n';
        return false;
    }

    else if (TTF_Init() == -1)
    {
        std::cout << "TTF initialisation unsuccessful! TTF_Error: " << TTF_GetError() << '\n';
        return false;
    }

    else
    {
        if (!g_window.init())
        {
            std::cout << "Window could not be initialised! SDL_Error: " << SDL_GetError() << '\n';
            return false;
        }

        else
        {
            g_renderer = g_window.createRenderer();
            g_font = TTF_OpenFont("Assets/Fonts/pixelFont.ttf", 24);

            if (!g_renderer)
            {
                std::cout << "Renderer could not be initialised! SDL_Error: " << SDL_GetError() << '\n';
                return false;
            }

            else if (!g_font)
            {
                std::cout << "Font could not be initialised! SDL_Error: " << TTF_GetError() << '\n';
                return false;
            }

            else
            {
                SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
                m_stateMachine.reset(new StateMachine{});

                return true;
            }
        }
    }
}

void Game::close()
{
    SDL_DestroyRenderer(g_renderer);
    g_renderer = nullptr;

    g_window.destroy();

    TTF_CloseFont(g_font);
    g_font = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::playGame()
{
    if (init())
    {
        while (m_stateMachine->getCurrentStateID() != GameState::EXIT)
        {
            m_stateMachine->gameLoop();
        }
    }

    close();
}