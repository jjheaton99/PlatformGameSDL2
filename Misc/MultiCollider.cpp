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

void MultiCollider::setPosition(int index, int x, int y)
{
    if (index < 0 || index >= static_cast<int>(m_colliders.size()))
    {
        std::cout << "Provided index out of range of multicollider vector!" << '\n';
        return;
    }

    m_colliders[index].setPosition(x, y);
}

void MultiCollider::setPositions(const Vector2D<double>& basePosition, const std::vector<Vector2D<double>>& positions)
{
    if (positions.size() != m_colliders.size())
    {
        std::cout << "Positions vector does not match size of colliders vector!" << '\n';
        return;
    }

    for (int i{ 0 }; i < static_cast<int>(m_colliders.size()); ++ i)
    {
        m_colliders[i].setPosition(basePosition + positions[i]);
    }
}

void MultiCollider::setDimensions(int index, int w, int h)
{
    if (index < 0 || index >= static_cast<int>(m_colliders.size()))
    {
        std::cout << "Provided index out of range of multicollider vector!" << '\n';
        return;
    }

    m_colliders[index].setDimensions(w, h);
}