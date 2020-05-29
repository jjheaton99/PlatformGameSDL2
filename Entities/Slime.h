#pragma once

#include "GroundedEnemy.h"

class Player;

class Slime : public GroundedEnemy
{
protected:
    void enemyControls(Character& player) override;

public:
    Slime(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/redSlime.png", 
        int colliderWidth = 50, int colliderHeight = 50, int hitPoints = 100, int damage = 1);
};

