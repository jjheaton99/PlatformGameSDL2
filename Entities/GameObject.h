#pragma once

#include "TextureObject.h"
#include "Collider.h"
#include "Camera.h"
#include "Random.h"
#include <vector>
#include <cmath>

//base class for gameplay objects
class GameObject : public TextureObject
{
public:
    enum class EnemyType
    {
        SLIME,
        SPIDER,
        BAT,
        FLOATING_SKULL,
        MAX_ENEMIES
    };

public:
    enum class ProjectileType
    {
        SPIDER,
        SKULL,
        NONE
    };

protected:
    Collider m_collider;

    Vector2D<double> m_velocity;

    virtual void setCollider() { m_collider.setPosition(m_position.getx(), m_position.gety()); }

public:
    GameObject(const char* fileName, double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0);
    GameObject(double xPos = 0, double yPos = 0, double xVel = 0, double yVel = 0, double colliderWidth = 0, double colliderHeight = 0);
    virtual ~GameObject();

    const Vector2D<double>& getVel() const { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D<double>{ xVel, yVel }; }
    void setVel(Vector2D<double> vel) { m_velocity = vel; }
    void setDirection(double angle) { m_velocity.rotate(angle - m_velocity.direction()); };
    void addVel(const Vector2D<double>& vel) { m_velocity.add(vel); }
    void addVel(double x, double y) { m_velocity.add(x, y); }
    void scaleVel(double factor) { m_velocity.scale(factor); }

    virtual void cameraDraw(const Camera& camera) const;
    const Collider& getCollider() const { return m_collider; }
};
