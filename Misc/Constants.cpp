#include "Constants.h"

int g_screenWidth{ 1600 };
int g_screenHeight{ 900 };

SDL_Renderer* g_renderer{ nullptr };
TTF_Font* g_font{ nullptr };

bool Settings::vSync{ false };
int Settings::masterVol{ 100 };
int Settings::sfxVol{ 100 };
int Settings::musicVol{ 100 };