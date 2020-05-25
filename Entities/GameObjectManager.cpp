#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{}

void GameObjectManager::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player)
{
    for (auto& enemy : m_groundedEnemies)
    {
        //if enemy is dead and hasnt been deleted yet
        if (enemy && enemy->isDead())
        {
            enemy.reset(nullptr);
        }
        if (enemy)
        {
            enemy->update(map, camera, player);
        }
    }
}

void GameObjectManager::cameraDraw(const Camera& camera) const
{
    for (auto& enemy : m_groundedEnemies)
    {
        if (enemy)
        {
            enemy->cameraDraw(camera);
        }
    }
}

//new enemies are spawned with zero velocity by default
void GameObjectManager::newEnemy(Enemy type, double xPos, double yPos)
{
    switch (type)
    {
    case GameObjectManager::Enemy::SLIME:
        m_groundedEnemies.push_back(std::make_unique<Slime>(xPos, yPos));
        break;
    default:
        break;
    }
}