#pragma once

#include "GameObject.h"

class GameObject;

class Player : public GameObject
{
private:
    Vector2D<double> m_velocity;

public:
    Player(const char* textureSheet, double xStartPos = 0, double yStartPos = 0, double vx = 0, double vy = 0);
    ~Player();

    virtual void update() override;
};
