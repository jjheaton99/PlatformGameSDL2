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
    Collider(int width, int height, int xPos = 0, int yPos = 0);
    ~Collider();

    void setPosition(int x, int y);
    void setDimensions(int w, int h);
    const SDL_Rect& getHitBox() const { return m_hitBox; }

    bool collideCheck(const Collider& collider) const;
    static double axisBoxOverlap(int pos1, int pos2, int size1, int size2);
};

