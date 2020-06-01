#pragma once

#include "Character.h"

class FlyingCharacter : public Character
{
protected:
    //tilemap collision handlers
    virtual bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    virtual bool edgeCheck(const Camera& camera) override;

public:
    FlyingCharacter(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0, int hitPoints = 0);
    virtual ~FlyingCharacter();
};

