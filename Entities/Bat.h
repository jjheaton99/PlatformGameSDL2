#pragma once

#include "FlyingEnemy.h"

class Player;

class Bat : public FlyingEnemy
{
protected:
    double m_acceleration{ 2.0 };
    double m_maxSpeed{ 15.0 };
    double m_rotateAccel{ 20.0 };

    void motion() override {};
    void enemyControls(const Character& player) override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 5.0, m_position.gety() + 5.0); }

public:
    Bat(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/bat.png", 
        int colliderWidth = 40, int colliderHeight = 40, int hitPoints = 40, int damage = 1);
};