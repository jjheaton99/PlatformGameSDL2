#pragma once

#include "SDL.h"
#include "Vector2D.h"

class Collider
{
private:
    SDL_Rect m_box;

public:
    Collider(int width, int height);
    ~Collider();

    void setPosition(int x, int y);
    void setDimensions(int w, int h);
    SDL_Rect& getBox() { return m_box; }

    bool collideCheck(Collider& collider);
};

