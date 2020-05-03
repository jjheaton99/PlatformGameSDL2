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
    Tile(const char* fileName = "Assets/background2.png", Type type = BACKGROUND, double xPos = 0, double yPos = 0, int size = 50);
    ~Tile();

    void setPos(int xPos, int yPos);
    int getSize() { return m_size; }
    Type getType() { return m_type; }
};

