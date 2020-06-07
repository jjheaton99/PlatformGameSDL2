#pragma once

#include "FlyingEnemy.h"

class Player;

class Bat : public FlyingEnemy
{
protected:
    double m_acceleration{ 2.0 };
    double m_maxSpeed{ 12.0 };

    void motion() override {};
    void animateSprite() override;
    void enemyControls(const Character& player) override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 25.0, m_position.gety() + 25.0); }

public:
    Bat(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/blackBat.png", 
        int colliderWidth = 40, int colliderHeight = 40, int hitPoints = 40, int spriteSheetCount = 10, int damage = 1);
};