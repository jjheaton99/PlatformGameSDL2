#pragma once

#include "Player.h"

class Enemy
{
protected:
    int m_damage;

    virtual void enemyControls(Player& player) = 0;
    virtual void attackPlayer(Player& player) = 0;

public:
    Enemy(int damage);

    virtual void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player) = 0;
};

