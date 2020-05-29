#pragma once

#include "SDL.h"
#include "TextureObject.h"
#include "Player.h"
#include <memory>

class UserInterface
{
private:
    std::shared_ptr<Player> m_player;

    TextureObject m_hearts{ "Assets/MiscTextures/hearts.png" };
    void updateHearts();

public:
    UserInterface(std::shared_ptr<Player> player);
    ~UserInterface();

    void update();
    void draw();
};

