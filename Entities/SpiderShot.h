#pragma once

#include "EnemyProjectile.h"

class SpiderShot : public EnemyProjectile
{
private:
    const int m_spriteSheetCount{ 6 };
    int m_animationStep{ 0 };
    std::vector<SDL_Rect> m_spriteRects;
    std::vector<SDL_Rect>::size_type m_spriteIndex{ 0 };

    const double m_speed{ 20.0 };

    bool m_collidedMap{ false };
    bool m_collidedPlayer{ false };
    void animateShot();

    void motion() override {};

    void setCollider() override { m_collider.setPosition(m_position.getx() + 33.33, m_position.gety() + 33.33); }

public:
    SpiderShot(std::shared_ptr<Character> spider, std::shared_ptr<Character> player);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera) override;
};

