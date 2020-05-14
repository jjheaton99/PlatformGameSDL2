#pragma once

#include "SDL.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include <vector>

class GameObjectManager
{
public:
    enum class Enemy
    {
        GROUNDED
    };

    enum class Projectile
    {

    };

private:
    std::vector<Character*> m_enemies{};

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera);
    void cameraDraw(const Camera& camera) const;

    void newEnemy(Enemy type, int xPos, int yPos);
    std::vector<Character*>& getEnemies() { return m_enemies; }
};

