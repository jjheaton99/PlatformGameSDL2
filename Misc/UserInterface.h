#pragma once

#include "SDL.h"
#include "TextureObject.h"
#include "Player.h"
#include <memory>

class UserInterface
{
private:
    std::shared_ptr<Player> m_player;

    TextureObject m_healthBar{ "Assets/UITextures/healthBar.png" };
    const SDL_Rect m_healthBarBackgroundSrcRect{ 96, 0, 96, 16 };
    SDL_Rect m_healthBarBackgroundDstRect{ 20, g_screenHeight - 60, 240, 40 };
    const SDL_Rect m_healthBarSrcRect{ 2, 2, 92, 12 };
    SDL_Rect m_healthBarDstRect{ 25, g_screenHeight - 55, 230, 30 };
    void updateHealthBar();
    void drawHealthBar();

    TextureObject m_boomerangCooldown{ "Assets/UITextures/boomerang.png" };
    TextureObject m_dodgeCooldown{ "Assets/UITextures/dodge.png" };

public:
    UserInterface(std::shared_ptr<Player> player);
    ~UserInterface();

    void update();
    void draw();
};

