#pragma once

#include "GroundedEnemy.h"

class Player;

class Slime : public GroundedEnemy
{
protected:
    void enemyControls(std::shared_ptr<Character> player) override;

    void animateSprite() override;
    void cycleWalkAnimation();
    void cycleJumpAnimation();
    void cycleLandAnimation();

    void setCollider() override { m_collider.setPosition(m_position.getx() + 15.0, m_position.gety() + 15.0); }
    
public:
    Slime(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/slime.png", 
        double colliderWidth = 70, double colliderHeight = 60, int hitPoints = 100, int spriteSheetCount = 19, int damage = 30);
};

