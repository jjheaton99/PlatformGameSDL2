#include "Game.h"

int main(int argc, char* args[])
{
    Game game{};

    game.gameLoop();

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