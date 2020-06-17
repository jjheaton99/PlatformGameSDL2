#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{}

void GameObjectManager::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Player> player)
{
    for (auto& enemy : m_enemies)
    {
        //if enemy is dead and hasnt been deleted yet
        if (enemy && enemy->isDead())
        {
            double moneySpeed{ 15.0 };
            int moneyDrops{ MTRandom::getRandomInt(5, 15) };
            for (int i{ 0 }; i < moneyDrops; ++i)
            {
                m_projectiles.push_back(std::make_unique<MoneyProjectile>(player, enemy->getPos().getx(), enemy->getPos().gety(),
                    MTRandom::getRandomDouble(-moneySpeed, moneySpeed), MTRandom::getRandomDouble(-moneySpeed, moneySpeed)));
            }
            enemy.reset();
        }
        if (enemy)
        {
            enemy->update(map, camera, player);
            newProjectile(enemy->getProjectile(), enemy, player);
        }
    }

    for (int i{ 0 }; i < static_cast<int>(m_projectiles.size()); ++i)
    {
        m_projectiles[i]->update(map, camera);
        if (m_projectiles[i]->isTerminated())
        {
            m_projectiles.erase(m_projectiles.begin() + i);
            --i;
        }
    }

    for (int i{ 0 }; i < static_cast<int>(m_items.size()); ++i)
    {
        m_items[i]->update(map, camera);

        if (m_items[i]->isShopItem())
        {
            m_items[i]->setPriceColour(player->getMoney());
        }

        if (player->isInteracting() && !player->pickedUpItem() && m_items[i]->getCollider().collideCheck(player->getCollider())
            && ((m_items[i]->isShopItem() && m_items[i]->getPrice() < player->getMoney()) || !m_items[i]->isShopItem()))
        {
            newItem(player->pickUpItem(*m_items[i]), false, player->getPos().getx() + 45, player->getPos().gety() + 45);
            m_items.erase(m_items.begin() + i);
        }
    }
}

void GameObjectManager::cameraDrawEnemies(const Camera& camera) const
{
    for (auto& enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->cameraDraw(camera);
        }
    }
}

void GameObjectManager::cameraDrawProjectiles(const Camera& camera) const
{
    for (auto& projectile : m_projectiles)
    {
        projectile->cameraDraw(camera);
    }
}

void GameObjectManager::cameraDrawItems(const Camera& camera) const
{
    for (auto& item : m_items)
    {
        item->cameraDraw(camera);
    }
}

//new enemies are spawned with zero velocity by default
void GameObjectManager::newEnemy(GameObject::EnemyType type, double xPos, double yPos)
{
    switch (type)
    {
    case GameObject::EnemyType::SLIME:
        m_enemies.push_back(std::make_shared<Slime>(xPos, yPos));
        break;
    case GameObject::EnemyType::SPIDER:
        m_enemies.push_back(std::make_shared<Spider>(xPos, yPos));
        break;
    case GameObject::EnemyType::BAT:
        m_enemies.push_back(std::make_shared<Bat>(xPos, yPos));
        break;
    case GameObject::EnemyType::FLOATING_SKULL:
        m_enemies.push_back(std::make_shared<FloatingSkull>(xPos, yPos));
        break;
    default:
        break;
    }
}

void GameObjectManager::newRandomEnemy(double xPos, double yPos)
{
    GameObject::EnemyType type{ static_cast<GameObject::EnemyType>(MTRandom::getRandomInt(0, static_cast<int>(GameObject::EnemyType::MAX_ENEMIES) - 1)) };
    
    switch (type)
    {
    case GameObject::EnemyType::SLIME:
        m_enemies.push_back(std::make_shared<Slime>(xPos, yPos));
        break;
    case GameObject::EnemyType::SPIDER:
        m_enemies.push_back(std::make_shared<Spider>(xPos, yPos));
        break;
    case GameObject::EnemyType::BAT:
        m_enemies.push_back(std::make_shared<Bat>(xPos, yPos));
        break;
    case GameObject::EnemyType::FLOATING_SKULL:
        m_enemies.push_back(std::make_shared<FloatingSkull>(xPos, yPos));
        break;
    default:
        break;
    }
}

void GameObjectManager::newProjectile(GameObject::ProjectileType type, std::shared_ptr<Character> enemy, std::shared_ptr<Player> player)
{
    switch (type)
    {
    case GameObject::ProjectileType::SPIDER:
        m_projectiles.push_back(std::make_unique<SpiderShot>(enemy, player));
        break;
    case GameObject::ProjectileType::SKULL:
        m_projectiles.push_back(std::make_unique<FloatingSkullShot>(enemy, player));
        break;
    case GameObject::ProjectileType::NONE:
        break;
    default:
        break;
    }
}

void GameObjectManager::newItem(GameObject::ItemType type, bool shopItem, double xPos, double yPos)
{
    if (type != GameObject::ItemType::NONE)
    {
        m_items.push_back(std::make_unique<Item>(type, shopItem, xPos, yPos));
    }
}