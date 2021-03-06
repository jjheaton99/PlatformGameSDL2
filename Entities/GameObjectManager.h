#pragma once

#include "SDL.h"
#include "GameObject.h"
#include "Character.h"
#include "GroundedEnemy.h"
#include "MoneyProjectile.h"
#include "FlyingEnemy.h"
#include "Player.h"
#include "Slime.h"
#include "Spider.h"
#include "Bat.h"
#include "FloatingSkull.h"
#include "Projectile.h"
#include "SpiderShot.h"
#include "FloatingSkullShot.h"
#include "Item.h"
#include <vector>
#include <memory>

//manages the creation, drawing, updating and destruction of objects other than tiles or the player
class GameObjectManager
{
private:
    std::vector<std::shared_ptr<Character>> m_enemies{};
    std::vector<std::unique_ptr<Projectile>> m_projectiles{};
    std::vector<std::unique_ptr<Item>> m_items{};

    void newProjectile(GameObject::ProjectileType type, std::shared_ptr<Character> enemy, std::shared_ptr<Player> player);

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Player> player);
    void cameraDrawEnemies(const Camera& camera) const;
    void cameraDrawProjectiles(const Camera& camera) const;
    void cameraDrawItems(const Camera& camera) const;

    void newEnemy(GameObject::EnemyType type, double xPos, double yPos);
    void newRandomEnemy(double xPos, double yPos);
    std::vector<std::shared_ptr<Character>>& getEnemies() { return m_enemies; }
    void newItem(GameObject::ItemType type, bool shopItem, double xPos, double yPos);
};

