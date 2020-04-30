#pragma once

#include "Vector2D.h"
#include "TextureW.h"
#include "Collider.h"
#include <vector>

class GameObject
{
protected:
    Vector2D<double> m_position;

    TextureW m_texture;
    SDL_Rect m_srcRect;
    SDL_Rect m_dstRect;

    Collider m_collider;

public:
    GameObject(const char* fileName, double xPos = 0, double yPos = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~GameObject();

    virtual void draw();
    virtual void destroy();
    Collider& getCollider() { return m_collider; }
};
