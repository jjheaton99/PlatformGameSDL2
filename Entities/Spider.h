#pragma once

#include "GroundedEnemy.h"

class Player;

class Spider : public GroundedEnemy
{
protected:
    void enemyControls(std::shared_ptr<Character> player) override;

    void animateSprite() override;
    void cycleWalkAnimation() override;

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;

    void motion() override;
    void setCollider() override { m_collider.setPosition(m_position.getx(), m_position.gety()); }

public:
    Spider(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/slime.png",
        int colliderWidth = 70, int colliderHeight = 60, int hitPoints = 20, int spriteSheetCount = 19, int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) override;
};

