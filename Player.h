#pragma once

#include "GameObject.h"
#include "Constants.h"
#include <cstdint>

class GameObject;

class Player : public GameObject
{
private:
    Vector2D<double> m_velocity;
    bool m_airborne{ false };

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    virtual void update() override;
    const Vector2D<double>& getVel() { return m_velocity; }
    void setVel(double xVel, double yVel);
    void setDirection(double angle);
    bool isAirborne() { return m_airborne; }
    void makeAirborne() { m_airborne = true; }
};
