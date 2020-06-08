#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{}

GameObjectManager::~GameObjectManager()
{}

void GameObjectManager::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Player& player)
{
    int enemyCount{ 0 };
    for (auto& enemy : m_enemies)
    {
        //if enemy is dead and hasnt been deleted yet
        if (enemy && enemy->isDead())
        {
            enemy.reset();
        }
        if (enemy)
        {
            enemy->update(map, camera, player);
            ++enemyCount;
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
    case GameObjectManager::Enemy::SLIME:
        m_enemies.push_back(std::make_shared<Slime>(xPos, yPos));
        break;
    case GameObjectManager::Enemy::BAT:
        m_enemies.push_back(std::make_shared<Bat>(xPos, yPos));
        break;
    default:
        break;
    }
}

void GameObjectManager::newRandomEnemy(double xPos, double yPos)
{
    Enemy type{ static_cast<Enemy>(MTRandom::getRandomInt(0, static_cast<int>(Enemy::MAX_ENEMIES) - 1)) };
    
    switch (type)
    {
    case GameObjectManager::Enemy::SLIME:
        m_enemies.push_back(std::make_shared<Slime>(xPos, yPos));
        break;
    case GameObjectManager::Enemy::BAT:
        m_enemies.push_back(std::make_shared<Bat>(xPos, yPos));
        break;
    default:
        break;
    }
}