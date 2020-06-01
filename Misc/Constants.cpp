#include "Constants.h"

std::shared_ptr<WTexture> TileTextures::background{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> TileTextures::block{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> TileTextures::platform{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> TileTextures::ladder{ std::make_shared<WTexture>() };

int g_screenWidth{ 1600 };
int g_screenHeight{ 900 };

SDL_Renderer* g_renderer{ nullptr };