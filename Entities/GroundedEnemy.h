#pragma once

#include "GroundedCharacter.h"

class Player;

class GroundedEnemy : public GroundedCharacter
{
protected:
    int m_damage{ 1 };

    void enemyControls(Player& player);
    void motion() override;
    void attackPlayer(Player& player);

public:
    GroundedEnemy(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/redSlime.png", int hitPoints = 5);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player);
};

