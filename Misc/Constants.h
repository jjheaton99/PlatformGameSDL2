#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

namespace Constants
{
    const double pi{ 3.14159265359 };
    //g defines acceleration due to gravity for game objects
    const double g{ 1.5 };
    //the time between game logic updates
    const double updateStep{ 1.0 / 60.0 };
    const int tileSize{ 60 };
    const int chunkWidth{ 35 };
    const int chunkHeight{ 35 };
}

extern int g_screenWidth;
extern int g_screenHeight;
//convenient to have a global renderer that any object can use
extern SDL_Renderer* g_renderer;
extern TTF_Font* g_font;