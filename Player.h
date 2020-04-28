#pragma once

#include "Character.h"
#include "Constants.h"
#include <cmath>
#include <cstdint>

class GameObject;

class Player : public Character
{
public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    virtual void update(double timeStep) override;
};
