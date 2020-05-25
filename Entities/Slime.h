#pragma once

#include "GroundedEnemy.h"

class Player;

class Slime : public GroundedEnemy
{
protected:
    void enemyControls(Player& player) override;

public:
    Slime(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/redSlime.png", int hitPoints = 5, int damage = 1);
};

