#pragma once

#include "Projectile.h"
#include "Character.h"

class PlayerRangedAttack : public Projectile
{
protected:
    ItemType m_itemType;

    bool m_shootLeft{ false };
    bool m_shootRight{ false };
    bool m_flying{ false };
    double m_coolDown{ 3.0 };
    int m_coolDownCount{ 0 };
    bool m_isCooling{ false };

    virtual bool enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies) = 0;

public:
    PlayerRangedAttack(std::string fileName, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0.0, 
        double colliderHeight = 0.0, int damage = 1);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override {};
    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player) = 0;
    void cameraDraw(const Camera& camera) const override;

    bool isFlying() const { return m_flying; }

    virtual void shootLeft();
    virtual void shootRight();

    double getCooldownFraction() const;

    ItemType getType() const { return m_itemType; }
};

