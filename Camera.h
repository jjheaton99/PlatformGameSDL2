#pragma once

#include "SDL.h"
#include "Collider.h"

class Camera
{
private:
    Collider m_collider;
    SDL_Rect m_rect;
    int m_xBoundary;
    int m_yBoundary;

public:
    Camera(int width = Constants::screenWidth, int height = Constants::screenHeight,
        int xpos = 0, int ypos = 0, int xBoundary = 0, int yBoundary = 0);
    ~Camera();

    void setPos(int xpos, int ypos);
    void setBoundary(int xBoundary, int yBoundary);

    int getx() { return m_rect.x; }
    int gety() { return m_rect.y; }
    int getxBoundary() { return m_xBoundary; }
    int getyBoundary() { return m_yBoundary; }
    int getxMax() { return m_xBoundary - m_rect.w; }
    int getyMax() { return m_yBoundary - m_rect.h; }

    bool xInBoundary();
    bool yInBoundary();

    Collider& getCollider() { return m_collider; }
};
