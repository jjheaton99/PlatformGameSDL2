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
    Uint8 m_ID;
    Type m_type;
    
public:
    Tile(const char* fileName, double xPos, double yPos, Type type);
    ~Tile();

    virtual void update(double timeStep) override;
    void setDrawPos(double xPos, double yPos);

    Uint8 getID() { return m_ID; }
};

