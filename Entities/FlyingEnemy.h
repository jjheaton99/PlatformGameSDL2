#pragma once

#include "FlyingCharacter.h"

class Player;

class FlyingEnemy : public FlyingCharacter
{
protected:
    double m_updateRange{ 1000.0 };

    int m_damage;

    virtual void enemyControls(const Character& player) = 0;
    virtual bool attackPlayer(Character& player);

public:
    FlyingEnemy(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, 
        double yVel = 0, int colliderWidth = 0, int colliderHeight = 0, int hitPoints = 5, int spriteSheetCount = 1, int damage = 1);

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player) override;
};