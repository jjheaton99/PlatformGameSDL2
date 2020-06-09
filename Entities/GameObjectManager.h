#pragma once

#include "SDL.h"
#include "GameObject.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include "FlyingEnemy.h"
#include "Player.h"
#include "Slime.h"
#include "Bat.h"
#include "FloatingSkull.h"
#include "Projectile.h"
#include "FloatingSkullShot.h"
#include <vector>
#include <memory>

//manages the creation, drawing, updating and destruction of objects other than tiles or the player
class GameObjectManager
{
private:
    std::vector<std::shared_ptr<Character>> m_enemies{};
    std::vector<std::unique_ptr<Projectile>> m_projectiles{};

    void newProjectile(GameObject::ProjectileType type, std::shared_ptr<Character> enemy, std::shared_ptr<Character> player);

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player);
    void cameraDraw(const Camera& camera) const;

    void newEnemy(GameObject::EnemyType type, double xPos, double yPos);
    void newRandomEnemy(double xPos, double yPos);
    std::vector<std::shared_ptr<Character>>& getEnemies() { return m_enemies; }
};

