#pragma once

#include "SDL.h"
#include "TextureObject.h"
#include "Player.h"
#include <memory>

class UserInterface
{
private:
    std::shared_ptr<Player> m_player;

    TextureObject m_hearts{ "Assets/UITextures/hearts.png" };
    void updateHearts();

    TextureObject m_boomerangCooldown{ "Assets/UITextures/boomerang.png" };
    TextureObject m_dodgeCooldown{ "Assets/UITextures/dodge.png" };

public:
    UserInterface(std::shared_ptr<Player> player);
    ~UserInterface();

    void update();
    void draw();
};

