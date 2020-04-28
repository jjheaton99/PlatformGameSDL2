#pragma once

#include "GameObject.h"
#include "Constants.h"
#include <cmath>
#include <cstdint>

class GameObject;

class Player : public GameObject
{
private:
    Vector2D<double> m_velocity;
    bool m_airborne{ true };
    bool m_left{ false };
    bool m_right{ false };

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    virtual void update() override;

    const Vector2D<double>& getVel() { return m_velocity; }
    void setVel(double xVel, double yVel);
    void setDirection(double angle);

    bool isAirborne() { return m_airborne; }
    void makeAirborne() { m_airborne = true; }

    void moveLeft() 
    { 
        m_left = true;
        m_right = false;
    }
    void moveRight()
    {
        m_right = true;
        m_left = false;
    }
    void stop()
    {
        m_left = false;
        m_right = false;
    }

    void motion();
    void edgeCheck();
};
