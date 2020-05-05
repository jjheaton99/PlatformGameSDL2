#include "Game.h"

SDL_Renderer* TextureW::renderer{ nullptr };

Game::Game()
{
    init();
}

Game::~Game()
{
    close();
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL initialisation unsuccessful! SDL_Error: " << SDL_GetError() << '\n';
    }

    else
    {
        window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::screenWidth, Constants::screenHeight, 0);
        if (!window)
        {
            std::cout << "Window not created! SDL_Error: " << SDL_GetError() << '\n';
        }

        else
        {
            TextureW::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!TextureW::renderer)
            {
                std::cout << "Renderer not created! SDL_Error: " << SDL_GetError() << '\n';
            }

            else
            {
                SDL_SetRenderDrawColor(TextureW::renderer, 0, 0, 0, 0);
            }
        }

        stateMachine = new StateMachine{ /*GameState::PLAY_GAME*/ };
    }
}

void Game::close()
{
    SDL_DestroyWindow(window);
    window = nullptr;

    delete stateMachine;
    stateMachine = nullptr;

    SDL_DestroyRenderer(TextureW::renderer);
    TextureW::renderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}

void Game::gameLoop()
{
    while (stateMachine->getCurrentStateID() != GameState::EXIT)
    {
        stateMachine->gameLoop();
        if (stateMachine->hasSetFullscreen())
        {
            if (!m_isFullscreen)
            {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                m_isFullscreen = true;
                stateMachine->resetSetFullscreen();
            }
            else if (m_isFullscreen)
            {
                SDL_SetWindowFullscreen(window, 0);
                m_isFullscreen = false;
                stateMachine->resetSetFullscreen();
            }
        }
    }
}