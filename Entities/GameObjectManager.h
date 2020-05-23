#pragma once

#include "SDL.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include "Player.h"
#include <vector>
#include <memory>

//manages the creation, drawing, updating and destruction of objects other than tiles or the player
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
    std::vector<std::unique_ptr<GroundedEnemy>> m_groundedEnemies{};

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player);
    void cameraDraw(const Camera& camera) const;

    void newEnemy(Enemy type, double xPos, double yPos);
    std::vector<std::unique_ptr<GroundedEnemy>>& getEnemies() { return m_groundedEnemies; }
};

