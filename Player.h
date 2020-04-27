#pragma once

#include "GameObject.h"

class GameObject;

class Player : public GameObject
{
private:
    Vector2D<double> m_velocity;

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    virtual void update() override;
};
