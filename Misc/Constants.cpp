#include "Constants.h"

int g_screenWidth{ 1600 };
int g_screenHeight{ 900 };

SDL_Renderer* g_renderer{ nullptr };
TTF_Font* g_font{ nullptr };

bool Settings::fullscreen;
bool Settings::vSync;
int Settings::masterVol;
int Settings::sfxVol;
int Settings::musicVol;