#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Tile.h"
#include <cmath>
#include <cstdint>

class Character : public GameObject
{
public:
    enum Movement
    {
        AIRBORNE,
        LEFT,
        RIGHT,
        STOP
    };

protected:
    Vector2D<double> m_velocity;
    Movement m_movement{ AIRBORNE };
    double m_xMaxSpeed{ 0 };

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~Character();

    virtual void update(double timeStep, std::vector<std::vector<Tile>>& map) = 0;

    const Vector2D<double>& getVel() { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D{ xVel, yVel }; }
    void setDirection(double angle) { m_velocity.rotate(angle); }

    Movement getMovement() { return m_movement; }
    void makeAirborne() { m_movement = AIRBORNE; }
    void moveLeft() { m_movement = LEFT; }
    void moveRight() { m_movement = RIGHT; }
    void stop() { m_movement = STOP; }

    virtual void motion(double acceleration, double deceleration);
    virtual void edgeCheck();
    virtual SDL_Rect getCollideTile(std::vector<std::vector<Tile>>& map);
    virtual void mapCollideCheck(std::vector<std::vector<Tile>>& map);
};

