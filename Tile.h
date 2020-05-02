#pragma once

#include "GameObject.h"

class Tile : public GameObject
{
public:
    enum Type
    {
        BACKGROUND,
        SOLID
    };

private:
    Type m_type;
    int m_size;
    
public:
    Tile(const char* fileName, double xPos, double yPos, Type type);
    ~Tile();

    void setPos(int xPos, int yPos);
    int getSize() { return m_size; }
    Type getType() { return m_type; }
};

