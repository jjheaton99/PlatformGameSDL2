#pragma once

#include "GameObject.h"

class GameObject;

class Player : public GameObject
{
public:
    Player(const char* textureSheet, int xStartPos, int yStartPos);
    ~Player();

    virtual void update() override;
};
