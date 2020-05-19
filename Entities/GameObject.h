#pragma once

#include "TextureObject.h"
#include "Collider.h"
#include "Camera.h"
#include <vector>
#include <cmath>

//base class for gameplay objects
class GameObject : public TextureObject
{
protected:
    Collider m_collider;

public:
    GameObject(const char* fileName, double xPos = 0, double yPos = 0, double colliderWidth = 0, double colliderHeight = 0);
    virtual ~GameObject();

    virtual void cameraDraw(const Camera& camera) const;
    const Collider& getCollider() const { return m_collider; }
};
