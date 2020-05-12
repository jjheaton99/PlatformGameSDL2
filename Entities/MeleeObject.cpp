#include "MeleeObject.h"

MeleeObject::MeleeObject(const char* fileName, double xBase, double yBase, int colliderWidth, int colliderHeight, double attackDuration)
    : GameObject(fileName, xBase, yBase, colliderWidth, colliderHeight), m_attackDuration{attackDuration}
{}

MeleeObject::~MeleeObject()
{
    destroy();
}

void MeleeObject::cameraDraw(Camera& camera)
{
    if (m_attacking)
    {
        SDL_Point rotationPoint{ m_offset.getx(), m_offset.gety() };
        if (m_collider.collideCheck(camera.getCollider()))
        {
            SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
            m_texture.draw(m_srcRect, relativeDstRect, m_angle, &rotationPoint, SDL_FLIP_NONE);
        }
    }
}
