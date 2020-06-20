#pragma once
#include "FlyingEnemy.h"

class Player;

class FloatingSkull : public FlyingEnemy
{
protected:
    const double m_acceleration{ 0.1 };
    const double m_maxSpeed{ 6.0 };

    SoundEffect m_skullShotSound{ "Assets/Sounds/EnemySounds/skullShot.wav" };

    void motion() override {};
    void animateSprite() override;
    void enemyControls(const std::shared_ptr<Character> player) override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 55.0, m_position.gety() + 55.0); }

public:
    FloatingSkull(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, std::string fileName = "Assets/Enemies/skull.png",
        double colliderWidth = 40, double colliderHeight = 40, int hitPoints = 400, int spriteSheetCount = 16, int damage = 0);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player);
};

