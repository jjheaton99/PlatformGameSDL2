#pragma once

#include "GameObject.h"
#include "Constants.h"
#include <cmath>
#include <cstdint>

class Character : public GameObject
{
protected:
    Vector2D<double> m_velocity;

    bool m_airborne{ true };
    bool m_left{ false };
    bool m_right{ false };

    double m_xMaxSpeed{ 0 };

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Character();

    virtual void update(double timeStep) = 0;

    const Vector2D<double>& getVel() { return m_velocity; }
    void setVel(double xVel, double yVel);
    void setDirection(double angle);

    bool isAirborne() { return m_airborne; }
    void makeAirborne() { m_airborne = true; }

    void moveLeft();
    void moveRight();
    void stop();

    virtual void motion();
    virtual void edgeCheck();
};

