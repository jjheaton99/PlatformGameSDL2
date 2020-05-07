#pragma once

#include "Character.h"

class GroundedCharacter : public Character
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
    Movement m_movement{ AIRBORNE };
    double m_xMaxSpeed{ 0 };

    virtual void mapCollideCheck(std::vector<std::vector<Tile>>& map) override;
    virtual bool checkForPlatforms(std::vector<std::vector<Tile>>& map, int characterRow,
        int characterColumn, int tileSize, SDL_Rect& characterCollider);
    virtual void edgeCheck(Camera& camera) override;

public:
    GroundedCharacter(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~GroundedCharacter();

    Movement getMovement() { return m_movement; }
    void makeAirborne() { m_movement = AIRBORNE; }
    void moveLeft() { m_movement = LEFT; }
    void moveRight() { m_movement = RIGHT; }
    void stop() { m_movement = STOP; }
    virtual void motion(double acceleration, double deceleration, double maxVel);
};

