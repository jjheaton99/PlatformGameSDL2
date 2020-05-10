#pragma once

#include "Character.h"

//class for characters bound by gravity
class GroundedCharacter : public Character
{
public:
    //enum for current state of motion
    enum Movement
    {
        AIRBORNE,
        LEFT,
        RIGHT,
        STOP
    };

protected:
    Movement m_movement{ AIRBORNE };
    double m_walkAcceleration{ 0 };

    virtual void mapCollideCheck(std::vector<std::vector<Tile>>& map) override;
    virtual bool checkForGround(std::vector<std::vector<Tile>>& map, int characterRow,
        int characterColumn, int tileSize, SDL_Rect& characterColliderBox);
    virtual bool edgeCheck(Camera& camera) override;
    
    virtual void motion() = 0;

public:
    GroundedCharacter(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~GroundedCharacter();

    Movement getMovement() { return m_movement; }
    //functions for changing state of motion
    void makeAirborne() { m_movement = AIRBORNE; }
    void moveLeft() { m_movement = LEFT; }
    void moveRight() { m_movement = RIGHT; }
    void stop() { m_movement = STOP; }
    void addVel(const Vector2D<double>& vel) { m_velocity.add(vel); }
};

