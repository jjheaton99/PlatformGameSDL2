#pragma once

#include "GroundedCharacter.h"

class GroundedEnemy : public GroundedCharacter
{
protected:
    void enemyControls();
    void motion() override;

public:
    GroundedEnemy(const char* fileName = "Assets/Enemies/redSlime.png", double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera) override;
};

