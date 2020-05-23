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
        CLIMB_STOP,
        CLIMB_UP,
        CLIMB_DOWN,
        STOP
    };

protected:
    Movement m_movement{ AIRBORNE };
    double m_walkAcceleration{ 0 };
    double m_climbSpeed{ 0 };
    bool m_crouched{ false };
    bool m_hasCrouched{ false };

    //tilemap collision handlers
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    virtual bool checkForGround(const std::vector<std::vector<Tile>>& map, int characterRow,
        int characterColumn, const Collider::DoubleRect& characterColliderBox);
    virtual bool edgeCheck(const Camera& camera) override;

public:
    GroundedCharacter(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0, int hitPoints = 0);
    virtual ~GroundedCharacter();

    Movement getMovement() const { return m_movement; }
    //functions for changing state of motion
    void makeAirborne() { m_movement = AIRBORNE; }
    void moveLeft() { m_movement = LEFT; }
    void moveRight() { m_movement = RIGHT; }
    void stop() { m_movement = STOP; }

    void climbStop() { m_movement = CLIMB_STOP; }
    void climbUp() { m_movement = CLIMB_UP; }
    void climbDown() { m_movement = CLIMB_DOWN; }
    bool isClimbing() const { return (m_movement == CLIMB_STOP || m_movement == CLIMB_UP || m_movement == CLIMB_DOWN); }

    void crouch();
    void uncrouch() { m_crouched = false; }
};

