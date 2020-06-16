#pragma once

#include "GameObject.h"
#include "MultiCollider.h"
#include "Tile.h"
#include "Character.h"
#include "GroundedEnemy.h"

//a base class for directional melee attacks
class MeleeObject : public GameObject
{
protected:
    ItemType m_itemType;

    const double m_attackDuration;

    int m_damage;

    bool m_facingLeft{ false };
    bool m_attacking{ false };
    int m_counter{ 0 };
    int m_updateCount;

    double m_angle{ 0.0 };

    //for offsetting weapon from base position
    Vector2D<double> m_totalPosition{ 0, 0 };
    Vector2D<double> m_offset{ 0, 0 };

    std::vector<bool> m_hitEnemies;

public:
    MeleeObject(std::string fileName, int damage, double xBasePos, double yBasePos, double colliderWidth, double colliderHeight, double attackDuration = 0);
    virtual ~MeleeObject();

    virtual bool update(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel) = 0;
    virtual bool collideCheck(std::vector<std::shared_ptr<Character>>& enemies, const Vector2D<double>& playerVel, double xKnockback = 0, double yKnockback = 0);

    virtual void cameraDraw(const Camera& camera) const override;
    void attackLeft();
    void attackRight();
    void cancel();
    bool isAttacking() const { return m_attacking; }

    void updateHitEnemies(const std::vector<std::shared_ptr<Character>>& enemies);
    void resetHitEnemies();

    void playerDying() { m_texture.setAlpha(0); }

    ItemType getType() const { return m_itemType; }
};

