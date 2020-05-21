#include "Collider.h"

Collider::Collider(double width, double height, double xPos, double yPos)
{
    m_hitBox.w = width;
    m_hitBox.h = height;
    m_hitBox.x = xPos;
    m_hitBox.y = yPos;
}

Collider::~Collider()
{}

void Collider::setPosition(double x, double y)
{
    m_hitBox.x = x;
    m_hitBox.y = y;
}

void Collider::setPosition(const Vector2D<double>& position)
{
    m_hitBox.x = position.getx();
    m_hitBox.y = position.gety();
}

void Collider::setDimensions(double w, double h)
{
    m_hitBox.w = w;
    m_hitBox.h = h;
}

bool Collider::collideCheck(const Collider& collider) const
{
    //checks for overlap of both x and y axes between the colliders 
    return (m_hitBox.x < (collider.getHitBox().x + collider.getHitBox().w)
        && m_hitBox.x >(collider.getHitBox().x - m_hitBox.w)
        && m_hitBox.y < (collider.getHitBox().y + collider.getHitBox().h)
        && m_hitBox.y >(collider.getHitBox().y - m_hitBox.h));
}

//swept AABB collisions with map tiles to prevent high speed going through
std::pair<Collider::CollisionType, double> Collider::tileCollideCheck(const Vector2D<double>& velocity, const std::tuple<Collider, double, double>& tileCollider) const
{
    //closest and furthest distances between colliders in x and y directions
    double xCloseDist;
    double xFarDist;
    double yCloseDist;
    double yFarDist;

    DoubleRect tileHitBox{ std::get<0>(tileCollider).getHitBox() };

    //sets distances based on direction of motion
    if (velocity.getx() > 0.0)
    {
        xCloseDist = 1.0 * tileHitBox.x - m_hitBox.x - m_hitBox.w;
        xFarDist = 1.0 * tileHitBox.x + tileHitBox.w - m_hitBox.x;
    }
    else
    {
        xCloseDist = 1.0 * tileHitBox.x + tileHitBox.w - m_hitBox.x;
        xFarDist = 1.0 * tileHitBox.x - m_hitBox.x - m_hitBox.w;
    }

    if (velocity.gety() > 0.0)
    {
        yCloseDist = 1.0 * tileHitBox.y - m_hitBox.y - m_hitBox.h;
        yFarDist = 1.0 * tileHitBox.y + tileHitBox.h - m_hitBox.y;
    }
    else
    {
        yCloseDist = 1.0 * tileHitBox.y + tileHitBox.h - m_hitBox.y;
        yFarDist = 1.0 * tileHitBox.y - m_hitBox.y - m_hitBox.h;
    }

    //relative collision start and end times
    double xStartTime;
    double xEndTime;
    double yStartTime;
    double yEndTime;

    double xOverlap{ std::get<1>(tileCollider) };
    double yOverlap{ std::get<2>(tileCollider) };
    //colliding if overlaps are both positive
    bool colliding{ xOverlap > 0.0 && yOverlap > 0.0 };

    //no divide by zero
    if (velocity.getx() == 0.0)
    {
        if ( xOverlap > 0.0)
        {
            xStartTime = -std::numeric_limits<double>::infinity();
        }
        else
        {
            xStartTime = std::numeric_limits<double>::infinity();
        }
        xEndTime = std::numeric_limits<double>::infinity();
    }
    else
    {
        if (colliding)
        {
            xStartTime = - xCloseDist / velocity.getx();
        }
        else
        {
            xStartTime = xCloseDist / velocity.getx();
        }
        xEndTime = xFarDist / velocity.getx();
    }

    if (velocity.gety() == 0.0)
    {
        if (yOverlap > 0.0)
        {
            yStartTime = -std::numeric_limits<double>::infinity();
        }
        else
        {
            yStartTime = std::numeric_limits<double>::infinity();
        }
        yEndTime = std::numeric_limits<double>::infinity();
    }
    else
    {
        if (colliding)
        {
            yStartTime = - yCloseDist / velocity.gety();
        }
        else
        {
            yStartTime = yCloseDist / velocity.gety();
        }
        yEndTime = yFarDist / velocity.gety();
    }

    //collision start time is the longer of the x and y times
    double startTime{ std::max(xStartTime, yStartTime) };
    //collision end time is shorter of x and y times
    double endTime{ std::min(xEndTime, yEndTime) };


    //check collision occured in this frame
    if (startTime > endTime || xStartTime < 0.0 && yStartTime < 0.0 || xStartTime > 1.0 || yStartTime > 1.0)
    {
        return { NONE, startTime };
    }
    //if colliders are overlapping
    else if (colliding)
    {
        //for situations where character is forced into colliding with block
        if (yOverlap < xOverlap)
        {
            if (velocity.gety() > 0.0)
            {
                return { OVERLAP_TOP, startTime };
            }
            else
            {
                return { OVERLAP_BOTTOM, startTime };
            }
        }
        else
        {
            if (velocity.getx() > 0.0)
            {
                return { OVERLAP_LEFT, startTime };
            }
            else
            {
                return { OVERLAP_RIGHT, startTime };
            }
        }
    }
    //sort collision directions using start times and velocity
    else if (xStartTime < yStartTime)
    {
        if (velocity.gety() > 0.0)
        {
            return { TOP, startTime };
        }
        else
        {
            return { BOTTOM, startTime };
        }
    }
    else
    {
        if (velocity.getx() > 0.0)
        {
            return { LEFT, startTime };
        }
        else
        {
            return { RIGHT, startTime };
        }
    }
}

//for calculating x or y overlap between two hitboxes
double Collider::axisBoxOverlap(double pos1, double pos2, double size1, double size2)
{
    if (pos1 < pos2)
    {
        return 1.0 * pos1 + size1 - pos2;
    }
    else if (pos1 > pos2)
    {
        return 1.0 * pos2 + size2 - pos1;
    }
    else if (pos1 = pos2)
    {
        if (size1 > size2)
        {
            return size2;
        }
        else
        {
            return size1;
        }
    }
    else
    {
        return -1.0;
    }
}
