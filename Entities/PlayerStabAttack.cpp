#include "PlayerStabAttack.h"

PlayerStabAttack::PlayerStabAttack(int damage, double xBase, double yBase)
    : MeleeObject("Assets/Attacks/axe.png", damage, xBase, yBase, 0.0, 0.0, 0.35)
{
    m_srcRect = { 0, 0, 42, 15 };

    m_dstRect.w = 131;
    m_dstRect.h = 47;

    m_offset = Vector2D<double>{ 19.0, 22.0 };
}

void PlayerStabAttack::update(std::vector<std::shared_ptr<Character>>& enemies)
{
    updateHitEnemies(enemies);

    if (m_attacking)
    {

    }
}