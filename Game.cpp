#include "Game.h"

Game::Game()
{}

Game::~Game()
{}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL initialisation unsuccessful! SDL_Error: " << SDL_GetError() << '\n';
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
            if (g_renderer == nullptr)
            {
                std::cout << "Window could not be initialised! SDL_Error: " << SDL_GetError() << '\n';
                return false;
            }

            else
            {
                SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
                m_stateMachine = new StateMachine{};
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