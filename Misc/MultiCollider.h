#pragma once

#include "Collider.h"
#include <vector>

//class for creating colliders made of multiple colliders
class MultiCollider
{
private:
    std::vector<Collider> m_colliders;

public:
    MultiCollider() = default;
    MultiCollider(std::vector<Collider::DoubleRect> hitBoxes);
    ~MultiCollider();

    bool collideCheck(const Collider& collider) const;
    bool collideCheck(const MultiCollider& collider) const;
    const std::vector<Collider>& getColliders() const { return m_colliders; }
    void setPosition(int index, double x, double y);
    void setPositions(const Vector2D<double>& basePosition, const std::vector<Vector2D<double>>& positions);
    void setDimensions(int index, double w, double h);
};

