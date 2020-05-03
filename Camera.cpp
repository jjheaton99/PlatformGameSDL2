#include "Camera.h"

Camera::Camera(int width, int height, int xpos, int ypos, int xBoundary, int yBoundary)
    : m_collider{width, height}, m_rect{xpos, ypos, width, height}, m_xBoundary{xBoundary}, m_yBoundary{yBoundary}
{}

Camera::~Camera()
{}

void Camera::setPos(int xpos, int ypos)
{
    m_rect.x = xpos;
    m_rect.y = ypos;

    //move the collider
    m_collider.setPosition(xpos, ypos);
}

void Camera::setBoundary(int xBoundary, int yBoundary)
{
    m_xBoundary = xBoundary;
    m_yBoundary = yBoundary;
}

bool Camera::xInBoundary()
{
    return (m_rect.x >= 0 && m_rect.x <= m_xBoundary - m_rect.w);
}

bool Camera::yInBoundary()
{
    return (m_rect.y >= 0 && m_rect.y <= m_yBoundary - m_rect.h);
}