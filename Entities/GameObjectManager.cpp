#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{
    for (auto& enemy : m_enemies)
    {
        if (enemy)
        {
            delete enemy;
            enemy = nullptr;
        }
    }
}

void GameObjectManager::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, const Vector2D<double>& playerPos)
{
    for (auto& enemy : m_enemies)
    {
        //if enemy is dead and hasnt been deleted yet
        if (enemy && enemy->isDead())
        {
            delete enemy;
            enemy = nullptr;
        }
        if (enemy)
        {
            enemy->update(map, camera, playerPos);
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

//new enemies are spawned with zero velocity by default
void GameObjectManager::newEnemy(Enemy type, double xPos, double yPos)
{
    switch (type)
    {
    case GameObjectManager::Enemy::GROUNDED:
        m_enemies.push_back(new GroundedEnemy{"Assets/Enemies/redSlime.png", xPos, yPos });
        break;
    default:
        break;
    }
}