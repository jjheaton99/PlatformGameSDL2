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
        enemy->update(map, camera);
    }
}

void GameObjectManager::cameraDraw(const Camera& camera) const
{
    for (auto& enemy : m_enemies)
    {
        enemy->cameraDraw(camera);
    }
}
