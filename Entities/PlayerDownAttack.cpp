#include "PlayerDownAttack.h"

PlayerDownAttack::PlayerDownAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/axe.png", damage, xBase, yBase, 0.0, 0.0, 0.35)
{
    m_srcRect = { 0, 0, 0, 0 };

    m_dstRect.w = 0;
    m_dstRect.h = 0;

    m_offset = Vector2D<double>{ 0.0, 0.0 };
}

void PlayerDownAttack::update(std::vector<std::shared_ptr<Character>>& enemies)
{
    updateHitEnemies(enemies);

    if (m_attacking)
    {

    }
}