#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{

}

void GameObjectManager::update(const std::vector<std::vector<Tile>>& map, Camera& camera)
{
    for (auto& enemy : m_enemies)
    {
        if (enemy && enemy->isDead())
        {
            delete enemy;
            enemy = nullptr;
        }
        if (enemy)
        {
            enemy->update(map, camera);
        }
    }
}

void GameObjectManager::cameraDraw(const Camera& camera) const
{
    for (auto& enemy : m_enemies)
    {
        if (enemy)
        {
            enemy->cameraDraw(camera);
        }
    }
}

void GameObjectManager::newEnemy(Enemy type, int xPos, int yPos)
{
    switch (type)
    {
    case GameObjectManager::Enemy::GROUNDED:
        m_enemies.push_back(new GroundedEnemy{});
        break;
    default:
        break;
    }
}