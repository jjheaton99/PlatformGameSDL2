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

    using sweptObstacleTuple = std::tuple<Collider, double, double>;
    using sweptAABBresult_type = std::pair<CollisionType, double>;

private:
    DoubleRect m_hitBox;

    static double axisBoxOverlap(double pos1, double pos2, double size1, double size2);

public:
    Collider(double width, double height, double xPos = 0, double yPos = 0);
    ~Collider();

    void setPosition(double x, double y);
    void setPosition(const Vector2D<double>& position);
    void setDimensions(double w, double h);
    const DoubleRect& getHitBox() const { return m_hitBox; }

    bool collideCheck(const Collider& collider) const;
    sweptAABBresult_type sweptAABBCheck(const Vector2D<double>& velocity, const Vector2D<double>& obstacleVel, sweptObstacleTuple& obstacleTuple) const;

    static double xOverlap(const Collider& c1, const Collider& c2) { return axisBoxOverlap(c1.getHitBox().x, c2.getHitBox().x, c1.getHitBox().w, c2.getHitBox().w); }
    static double yOverlap(const Collider& c1, const Collider& c2) { return axisBoxOverlap(c1.getHitBox().y, c2.getHitBox().y, c1.getHitBox().h, c2.getHitBox().h); }
};
