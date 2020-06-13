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
    const SDL_Rect m_healthBarBackgroundSrcRect{ 111, 0, 111, 16 };
    SDL_Rect m_healthBarBackgroundDstRect{ 20, g_screenHeight - 80, 416, 60 };
    const SDL_Rect m_healthBarSrcRect{ 2, 2, 92, 12 };
    SDL_Rect m_healthBarDstRect{ 84, g_screenHeight - 72, 345, 45 };
    void updateHealthBar();
    void drawHealthBar();

    TextureObject m_healthPotion{ "Assets/UITextures/healthPotion.png" };
    TextureObject m_boomerangCooldown{ "Assets/UITextures/boomerang.png" };
    TextureObject m_dodgeCooldown{ "Assets/UITextures/dodge.png" };

public:
    UserInterface(std::shared_ptr<Player> player);
    ~UserInterface();

    void update();
    void draw();
};

