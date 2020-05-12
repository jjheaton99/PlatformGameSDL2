#pragma once

#include "SDL.h"
#include "Collider.h"

//Camera class for rendering portion of map that is on screen
class Camera
{
private:
    //collider to check if an object is on screen
    Collider m_collider;
    //sdl rect for camera size and position
    SDL_Rect m_rect;
    //boundaries are maximum displacements of camera
    int m_xBoundary;
    int m_yBoundary;

public:
    //camera size defaulted to screen dimensions
    Camera(int width = g_screenWidth, int height = g_screenHeight,
        int xpos = 0, int ypos = 0, int xBoundary = 0, int yBoundary = 0);
    ~Camera();

    void resize();
    void setPos(int xpos, int ypos);
    void setBoundary(int xBoundary, int yBoundary);

    int getx() const { return m_rect.x; }
    int gety() const { return m_rect.y; }
    int getw() const { return m_rect.w; }
    int geth() const { return m_rect.h; }
    int getxBoundary() const { return m_xBoundary; }
    int getyBoundary() const { return m_yBoundary; }
    int getxMax() const { return m_xBoundary - m_rect.w; }
    int getyMax() const { return m_yBoundary - m_rect.h; }

    bool xInBoundary() const;
    bool yInBoundary() const;

    const Collider& getCollider() const { return m_collider; }
};
