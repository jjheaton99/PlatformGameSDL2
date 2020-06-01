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
                TileTextures::background->load("Assets/MapTiles/blackGrey.png");
                TileTextures::block->load("Assets/MapTiles/WhiteFadeBlocks/1.png");
                TileTextures::platform->load("Assets/MapTiles/platform.png");
                TileTextures::ladder->load("Assets/MapTiles/ladder.png");

                SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0);
                m_stateMachine.reset(new StateMachine{});

                return true;
            }
        }
    }
}

void Game::close()
{
    TileTextures::background->destroy();
    TileTextures::block->destroy();
    TileTextures::platform->destroy();
    TileTextures::ladder->destroy();

    SDL_DestroyRenderer(g_renderer);

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