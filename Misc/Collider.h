#pragma once

#include "SDL.h"
#include "Vector2D.h"
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <utility>
#include <tuple>

//class for managing hitboxes and checking collisions between hitboxes
class Collider
{
public:
    enum CollisionType
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        OVERLAP_TOP,
        OVERLAP_BOTTOM,
        OVERLAP_LEFT,
        OVERLAP_RIGHT,
        NONE
    };

    struct DoubleRect
    {
        double x;
        double y;
        double w;
        double h;
    };

private:
    DoubleRect m_hitBox;

public:
    Collider(double width, double height, double xPos = 0, double yPos = 0);
    ~Collider();

    void setPosition(double x, double y);
    void setPosition(const Vector2D<double>& position);
    void setDimensions(double w, double h);
    const DoubleRect& getHitBox() const { return m_hitBox; }

    bool collideCheck(const Collider& collider) const;
    std::pair<CollisionType, double> tileCollideCheck(const Vector2D<double>& velocity, const std::tuple<Collider, double, double>& tileCollider) const;
    static double axisBoxOverlap(double pos1, double pos2, double size1, double size2);
};
