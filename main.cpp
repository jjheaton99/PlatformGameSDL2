#include "SDL.h"
#include "TextureW.h"
#include "StateMachine.h"
#include "Constants.h"
#include <iostream>
#include <vector>

SDL_Renderer* TextureW::renderer{ nullptr };
SDL_Window* window{ nullptr };
StateMachine* stateMachine{ nullptr };

void init(int flags)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialisation unsuccessful! SDL_Error: " << SDL_GetError() << '\n';
    }

    else
    {
        window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::screenWidth, Constants::screenHeight, flags);
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

        stateMachine = new StateMachine{ GameState::PLAY_GAME };
    }
}

void close()
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

int main(int argc, char* args[])
{
    int flags{ 0 };

    init(flags);

    while (stateMachine->getCurrentStateID() != GameState::EXIT)
    {
        stateMachine->gameLoop();
    }

    close();

    return 0;
}

/*
const int FPS{ 60 };
const int frameDelay{ 1000 / FPS };

Uint32 frameStart;
int frameTime;
*/

/*
frameStart = SDL_GetTicks();

game->handleEvents();
game->update();
game->render();

frameTime = SDL_GetTicks() - frameStart;

if (frameDelay > frameTime)
{
    SDL_Delay(frameDelay - frameTime);
}
*/