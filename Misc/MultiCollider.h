#pragma once

#include "Collider.h"
#include <vector>

class MultiCollider
{
private:
    std::vector<Collider> m_colliders;

public:
    MultiCollider() = default;
    MultiCollider(std::vector<SDL_Rect> hitBoxes);
    ~MultiCollider();

    bool collideCheck(const Collider& collider) const;
    bool collideCheck(const MultiCollider& collider) const;
    const std::vector<Collider>& getColliders() const { return m_colliders; }
    void setPosition(int index, int x, int y);
    void setPositions(const Vector2D<double>& basePosition, const std::vector<Vector2D<double>>& positions);
    void setDimensions(int index, int w, int h);
};

