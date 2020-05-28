#include "PlayerBoomerang.h"

PlayerBoomerang::PlayerBoomerang(double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0, const char* fileName = "Assets/Enemies/redSlime.png", int damage = 1)
    : Projectile(fileName, xPos, yPos, xVel, yVel, colliderWidth, colliderHeight), m_damage{damage}
{}

void PlayerBoomerang::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::unique_ptr<Character>>& enemies)
{

}

bool PlayerBoomerang::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{

}

void PlayerBoomerang::enemyCollideCheck(std::vector<std::unique_ptr<Character>>& enemies)
{

}

void PlayerBoomerang::motion()
{

}

void PlayerBoomerang::cameraDraw(const Camera& camera) const
{
    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, SDL_FLIP_NONE);
    }
}