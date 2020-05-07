#pragma once

#include "SDL.h"

namespace Constants
{
    const double pi{ 3.14159265359 };
    const double g{ 30.0 };
}

extern int g_screenWidth;
extern int g_screenHeight;

extern SDL_Renderer* g_renderer;