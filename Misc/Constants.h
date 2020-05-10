#pragma once

#include "SDL.h"

namespace Constants
{
    const double pi{ 3.14159265359 };
    //g defines acceleration due to gravity for game objects
    const double g{ 30.0 };
}

//some globals included for convenience
extern int g_screenWidth;
extern int g_screenHeight;

extern SDL_Renderer* g_renderer;