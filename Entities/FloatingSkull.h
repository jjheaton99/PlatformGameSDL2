#pragma once
#include "FlyingEnemy.h"

class Player;

class FloatingSkull : public FlyingEnemy
{
protected:
    double m_maxSpeed{ 12.0 };

    int m_projectileCount{ 0 };

    void motion() override {};
    void animateSprite() override;
    void enemyControls(const std::shared_ptr<Character> player) override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 55.0, m_position.gety() + 55.0); }

public:
    FloatingSkull(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/Enemies/skull.png",
        int colliderWidth = 40, int colliderHeight = 40, int hitPoints = 40, int spriteSheetCount = 16, int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player);
};

