#pragma once

#include "GroundedCharacter.h"

class Player;

class GroundedEnemy : public GroundedCharacter
{
protected:
    EnemyType m_type;

    int m_projectileCount{ 0 };
    int m_damage;

    double m_stopDeceleration{ 0.7 };

    //for animating jumping and landing
    bool m_jumped{ false };
    bool m_landed{ false };

    virtual void enemyControls(std::shared_ptr<Character> player) = 0;
    virtual void motion() override;
    virtual void attackPlayer(std::shared_ptr<Character> player);

public:
    GroundedEnemy(std::string fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, 
        double yVel = 0, double colliderWidth = 0, double colliderHeight = 0, int hitPoints = 5, int spriteSheetCount = 1, int damage = 1);

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) override;

    EnemyType getType() const { return m_type; }
};

