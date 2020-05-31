#pragma once

#include "SDL.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include "FlyingEnemy.h"
#include "Player.h"
#include "Slime.h"
#include "Bat.h"
#include <vector>
#include <memory>

//manages the creation, drawing, updating and destruction of objects other than tiles or the player
class GameObjectManager
{
public:
    enum class Enemy
    {
        SLIME,
        BAT
    };

    enum class Projectile
    {

    };

private:
    std::vector<std::shared_ptr<Character>> m_enemies{};

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const std::vector<std::vector<std::unique_ptr<Tile>>>& map, const Camera& camera, Player& player);
    void cameraDraw(const Camera& camera) const;

    void newEnemy(Enemy type, double xPos, double yPos);
    std::vector<std::shared_ptr<Character>>& getEnemies() { return m_enemies; }
};

