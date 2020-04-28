#include "Game.h"
#include "Vector2D.h"

Game* game = nullptr;
int main(int argc, char* args[])
{
    game = new Game();

    game->init("2D Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::screenWidth, Constants::screenHeight, false);

    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->close();

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