#pragma once

#include "GroundedEnemy.h"

class Player;

class Spider : public GroundedEnemy
{
protected:
    double m_runAwayRange{ MTRandom::getRandomDouble(400.0, 600.0) };

    void enemyControls(std::shared_ptr<Character> player) override;

    void animateSprite() override;
    void cycleWalkAnimation();
    void cycleIdleAnimation();

    bool checkForGround(const std::vector<std::vector<Tile>>& map, int characterRow,
        int characterColumn, const Collider::DoubleRect& characterColliderBox) override;
    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;

    void motion() override;
    void setCollider() override;

public:
    Spider(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/spider.png",
        double colliderWidth = 50, double colliderHeight = 30, int hitPoints = 20, int spriteSheetCount = 11, int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) override;
};

