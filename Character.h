#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Tile.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

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
    Vector2D<float> m_velocity;
    Movement m_movement{ AIRBORNE };
    float m_xMaxSpeed{ 0 };

public:
    Character(const char* fileName, float xStartPos = 0, float yStartPos = 0, float xVel = 0, float yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~Character();

    virtual void update(float timeStep, std::vector<std::vector<Tile>>& map) = 0;

    const Vector2D<float>& getVel() { return m_velocity; }
    void setVel(float xVel, float yVel) { m_velocity = Vector2D<float>{ xVel, yVel }; }
    void setDirection(float angle) { m_velocity.rotate(angle); }

    Movement getMovement() { return m_movement; }
    void makeAirborne() { m_movement = AIRBORNE; }
    void moveLeft() { m_movement = LEFT; }
    void moveRight() { m_movement = RIGHT; }
    void stop() { m_movement = STOP; }

    virtual void motion(float acceleration, float deceleration);
    virtual void edgeCheck();
    virtual std::vector<SDL_Rect> getCollideTileBoxes(std::vector<std::vector<Tile>>& map);
    virtual void mapCollideCheck(std::vector<std::vector<Tile>>& map);
    virtual bool checkForPlatforms(std::vector<std::vector<Tile>>& map);
};

