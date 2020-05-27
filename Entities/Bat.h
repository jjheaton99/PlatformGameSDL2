#pragma once

#include "FlyingEnemy.h"

class Player;

class Bat : public FlyingEnemy
{
protected:
    double m_acceleration{ 0.5 };
    double m_maxSpeed{ 10.0 };
    double m_rotateAccel{ 20.0 };

    void motion() override {};
    void enemyControls(Character& player) override;

public:
    Bat(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/bat.png", 
        int colliderWidth = 50, int colliderHeight = 50, int hitPoints = 5, int damage = 1);
};