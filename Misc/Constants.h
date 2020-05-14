#pragma once

#include "SDL.h"

namespace Constants
{
    const double pi{ 3.14159265359 };
    //g defines acceleration due to gravity for game objects
    const double g{ 1.25 };
    //the time between game logic updates
    const double updateStep{ 1.0 / 60.0 };
    const int tileSize{ 60 };
}

extern int g_screenWidth;
extern int g_screenHeight;
//convenient to have a global renderer that any object can use
extern SDL_Renderer* g_renderer;