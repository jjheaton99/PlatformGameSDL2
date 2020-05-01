#pragma once

#include "Constants.h"
#include <iostream>
#include <cmath>

template<class T>
class Vector2D
{
private:
    T m_x;
    T m_y;

public:
    Vector2D(T x = 0, T y = 0);
    ~Vector2D();

    T magnitude() const;
    void add(const Vector2D& vector);
    void subtract(const Vector2D& vector);
    void scale(T factor);
    void xScale(T factor);
    void yScale(T factor);
    T innerProduct(const Vector2D& vector);
    void rotate(T angle);

    T getx() const { return m_x; }
    T gety() const { return m_y; }
    void print() const;
};

template<class T>
Vector2D<T>::Vector2D(T x, T y)
    : m_x{ x }, m_y{ y }
{}

template<class T>
Vector2D<T>::~Vector2D()
{}

template<class T>
T Vector2D<T>::magnitude() const
{
    return static_cast<T>(std::sqrt(static_cast<double>(m_x) * static_cast<double>(m_x) + static_cast<double>(m_y) * static_cast<double>(m_y)));
}

template<class T>
void Vector2D<T>::add(const Vector2D& vector)
{
    m_x += vector.m_x;
    m_y += vector.m_y;
}

template<class T>
void Vector2D<T>::subtract(const Vector2D& vector)
{
    m_x -= vector.m_x;
    m_y -= vector.m_y;
}

template<class T>
void Vector2D<T>::scale(T factor)
{
    m_x *= factor;
    m_y *= factor;
}

template<class T>
void Vector2D<T>::xScale(T factor)
{
    m_x *= factor;
}

template<class T>
void Vector2D<T>::yScale(T factor)
{
    m_y *= factor;
}

template<class T>
T Vector2D<T>::innerProduct(const Vector2D& vector)
{
    return static_cast<T>(static_cast<double>(m_x) * static_cast<double>(vector.m_x) + static_cast<double>(m_y) * static_cast<double>(vector.m_y));
}

template<class T>
void Vector2D<T>::rotate(T angle)
{
    double radAngle{ (Constants::pi / 180.0) * static_cast<double>(angle) };
    T tempx{ m_x };

    m_x = static_cast<T>((m_x * std::cos(radAngle)) - (m_y * std::sin(radAngle)));
    m_y = static_cast<T>((tempx * std::sin(radAngle)) + (m_y * std::cos(radAngle)));
}

template<class T>
void Vector2D<T>::print() const
{
    std::cout << '(' << m_x << ", " << m_y << ")\n";
}

/*  For int non-template rotate function

    m_x = static_cast<int>(static_cast<float>(m_x) * std::cos(Constants::pi * (static_cast<float>(angle) / 180.0))
        - static_cast<float>(m_y) * std::sin(Constants::pi * (static_cast<float>(angle) / 180.0)));
    m_y = static_cast<int>(static_cast<float>(m_x) * std::sin(Constants::pi * (static_cast<float>(angle) / 180.0))
        + static_cast<float>(m_y) * std::cos(Constants::pi * (static_cast<float>(angle) / 180.0)));
*/

/*
std::ostream& operator<< (std::ostream& out, const Vector2D<int>& vector)
{
    out << "(" << vector.m_x << ", " << vector.m_y << ")";
    return out;
}
*/