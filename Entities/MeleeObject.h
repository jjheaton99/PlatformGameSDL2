#pragma once

#include "GameObject.h"

class MeleeObject : public GameObject
{
protected:
    const double m_attackDuration;

    bool m_facingLeft{ false };
    bool m_attacking{ false };
    int m_counter{ 0 };
    double m_angle{ 0.0 };

    //for offsetting weapon from base position
    Vector2D<double> m_totalPosition{ 0, 0 };
    Vector2D<double> m_offset{ 0, 0 };

public:
    MeleeObject(const char* fileName, double xBase, double yBase, int colliderWidth, int colliderHeight, double attackDuration = 0);
    virtual ~MeleeObject();

    virtual void update() = 0;

    virtual void cameraDraw(Camera& camera) override;
    void faceLeft() { m_facingLeft = true; }
    void faceRight() { m_facingLeft = false; }
    void attack() { m_attacking = true; }
    bool isAttacking() { return m_attacking; }
};

