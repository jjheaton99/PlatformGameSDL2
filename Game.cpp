#include "Game.h"

Player* player = nullptr;

SDL_Renderer* TextureManager::m_renderer = nullptr;

Game::Game()
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
            TextureManager::m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            if (!TextureManager::m_renderer)
            {
                std::cout << "Renderer not created! SDL_Error: " << SDL_GetError() << '\n';
            }

            else
            {
                SDL_SetRenderDrawColor(TextureManager::m_renderer, 255, 255, 255, 255);
            }
        }

        m_isRunning = true;
    }

    player = new Player("Assets/devil.png", 300, 700, 6, -25);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        m_isRunning = false;
    default:
        break;
    }
}

void Game::update()
{
    player->update();
}

void Game::render()
{
    SDL_RenderClear(TextureManager::m_renderer);
    player->render();
    SDL_RenderPresent(TextureManager::m_renderer);
}

void Game::close()
{
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_DestroyRenderer(TextureManager::m_renderer);
    TextureManager::m_renderer = nullptr;

    delete player;
    player = nullptr;

    IMG_Quit();
    SDL_Quit();
}
