#include "MultiCollider.h"

MultiCollider::MultiCollider(std::vector<Collider::DoubleRect> hitBoxes)
{
    for (const auto& hitBox : hitBoxes)
    {
        m_colliders.push_back(Collider{ hitBox.w, hitBox.h, hitBox.x, hitBox.y });
    }
}

MultiCollider::~MultiCollider()
{}

//collide check with single collider
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

//collide check with another multicollider
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

//individual collider position
void MultiCollider::setPosition(int index, double x, double y)
{
    if (index < 0 || index >= static_cast<int>(m_colliders.size()))
    {
        std::cout << "Provided index out of range of multicollider vector!" << '\n';
        return;
    }

    m_colliders[index].setPosition(x, y);
}

//takes a vector of positions for moving all the colliders
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

void MultiCollider::setDimensions(int index, double w, double h)
{
    if (index < 0 || index >= static_cast<int>(m_colliders.size()))
    {
        std::cout << "Provided index out of range of multicollider vector!" << '\n';
        return;
    }

    m_colliders[index].setDimensions(w, h);
}