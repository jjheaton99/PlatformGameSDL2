#include "GameObject.h"

GameObject::GameObject(const char* fileName, double xPos, double yPos, int colliderWidth, int colliderHeight)
    : TextureObject(fileName, xPos, yPos), m_collider{ colliderWidth, colliderHeight }
{}

GameObject::~GameObject()
{}

//a modified draw function for drawing objects relative to the camera
//ensures objects in camera focus are rendered in the correct place
void GameObject::cameraDraw(Camera& camera)
{
    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}