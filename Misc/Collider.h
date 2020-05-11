#pragma once

#include "SDL.h"
#include "Vector2D.h"
#include <vector>

//class for managing hitboxes and checking collisions between hitboxes
class Collider
{
private:
    SDL_Rect m_hitBox;

public:
    Collider(int width, int height);
    ~Collider();

    void setPosition(int x, int y);
    void setDimensions(int w, int h);
    SDL_Rect& getHitBox() { return m_hitBox; }

    bool collideCheck(Collider& collider);
    static double axisBoxOverlap(int pos1, int pos2, int size1, int size2);
};

