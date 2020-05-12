#pragma once

#include "Collider.h"
#include <vector>

class MultiCollider
{
private:
    std::vector<Collider> m_colliders;

public:
    MultiCollider(std::vector<SDL_Rect> hitBoxes);
    ~MultiCollider();

    bool collideCheck(const Collider& collider) const;
    bool collideCheck(const MultiCollider& collider) const;
    const std::vector<Collider>& getColliders() const { return m_colliders; }
};

