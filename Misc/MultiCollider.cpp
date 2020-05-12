#include "MultiCollider.h"

MultiCollider::MultiCollider(std::vector<SDL_Rect> hitBoxes)
{
    for (const auto& hitBox : hitBoxes)
    {
        m_colliders.push_back(Collider{ hitBox.w, hitBox.h, hitBox.x, hitBox.y });
    }
}

MultiCollider::~MultiCollider()
{}

bool MultiCollider::collideCheck(const Collider& collider) const
{
    for (const auto& colliderElement : m_colliders)
    {
        if (colliderElement.collideCheck(collider))
        {
            return true;
        }
    }

    return false;
}

bool MultiCollider::collideCheck(const MultiCollider& collider) const
{
    for (const auto& colliderElement1 : m_colliders)
    {
        for (const auto& colliderElement2 : collider.getColliders())
        {
            if (colliderElement1.collideCheck(colliderElement2))
            {
                return true;
            }
        }
    }

    return true;
}