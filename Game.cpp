#include "Game.h"

Player* player = nullptr;

Map* map = nullptr;

Map::layout_type lvl1{ {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
} };

SDL_Renderer* TextureW::m_renderer = nullptr;

static std::vector<SDL_Event> events;

TimerW stepTimer;

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
            TextureW::m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            if (!TextureW::m_renderer)
            {
                std::cout << "Renderer not created! SDL_Error: " << SDL_GetError() << '\n';
            }

            else
            {
                SDL_SetRenderDrawColor(TextureW::m_renderer, 0, 0, 0, 0);
            }
        }

        m_isRunning = true;
    }

    player = new Player("Assets/MrPix.png", 200, 200, 0, 0);

    map = new Map(lvl1);
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        events.push_back(event);
    }

    InputHandler::playerControlsKeyHold(player);
 
    for (auto& element : events)
    {
        if (!(InputHandler::windowEvent(element)))
        {
            m_isRunning = false;
        }

        InputHandler::playerControlsKeyPress(player, element);
    }

    events.clear();
}

void Game::update()
{
    float timeStep = stepTimer.getTicks() / 1000.0;
    //std::cout << timeStep << '\n';

    player->update(timeStep, map->getMap());

    stepTimer.start();
}

void Game::render()
{
    SDL_SetRenderDrawColor(TextureW::m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(TextureW::m_renderer);

    map->drawMap();
    player->draw();

    SDL_RenderPresent(TextureW::m_renderer);
}

void Game::close()
{
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_DestroyRenderer(TextureW::m_renderer);
    TextureW::m_renderer = nullptr;
    
    delete player;
    player = nullptr;

    delete map;
    map = nullptr;

    IMG_Quit();
    SDL_Quit();
}
