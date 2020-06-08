#pragma once

#include "GroundedCharacter.h"

class Player;

class GroundedEnemy : public GroundedCharacter
{
protected:
    double m_updateRange{ 1000.0 };

    int m_damage;

    double m_stopDeceleration{ 0.7 };

    //for animating jumping and landing
    bool m_jumped{ false };
    bool m_landed{ false };
    virtual void cycleWalkAnimation() = 0;
    virtual void cycleJumpAnimation() = 0;
    virtual void cycleLandAnimation() = 0;

    virtual void enemyControls(Character& player) = 0;
    virtual void motion() override;
    virtual void attackPlayer(Character& player);

public:
    GroundedEnemy(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, 
        double yVel = 0, int colliderWidth = 0, int colliderHeight = 0, int hitPoints = 5, int spriteSheetCount = 1, int damage = 1);

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player) override;
};

