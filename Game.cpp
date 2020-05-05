#include "Game.h"

//SDL_Renderer* TextureW::renderer = nullptr;

Game::Game()
    : m_stateMachine{ new StateMachine{GameState::PLAY_GAME} }
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags{ 0 };
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialisation unsuccessful! SDL_Error: " << SDL_GetError() << '\n';
        m_isRunning = false;
    }

    else
    {
        m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (!m_window)
        {
            std::cout << "Window not created! SDL_Error: " << SDL_GetError() << '\n';
        }

        else
        {
            TextureW::renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            if (!TextureW::renderer)
            {
                std::cout << "Renderer not created! SDL_Error: " << SDL_GetError() << '\n';
            }

            else
            {
                SDL_SetRenderDrawColor(TextureW::renderer, 0, 0, 0, 0);
            }
        }

        m_isRunning = true;
    }
}

void Game::singleLoop()
{
    m_stateMachine->gameLoop();
    if(m_stateMachine->getCurrentStateID() == GameState::EXIT)
    {
        m_isRunning = false;
    }
}

void Game::close()
{
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    delete m_stateMachine;
    m_stateMachine = nullptr;

    SDL_DestroyRenderer(TextureW::renderer);
    TextureW::renderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}
