#pragma once

#include "GroundedCharacter.h"

class Player;

class GroundedEnemy : public GroundedCharacter
{
protected:
    int m_damage;

    virtual void enemyControls(Player& player) = 0;
    virtual void motion() override;
    virtual void attackPlayer(Player& player);

public:
    GroundedEnemy(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0,  int hitPoints = 5, int damage = 1);

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player);
};

