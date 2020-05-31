#pragma once

#include "GameObject.h"

//class for tiles that construct the map
class Tile : public GameObject
{
public:
    enum Type
    {
        BACKGROUND,
        SOLID,
        PLATFORM,
        LADDER
    };

private:
    Type m_type;
    int m_size;
    
public:
    Tile(const char* fileName = "Assets/MapTiles/blackGrey.png", Type type = BACKGROUND, double xPos = 0, double yPos = 0, int size = Constants::tileSize);

    void setPos(double xPos, double yPos) override;
    int getSize() const { return m_size; }
    Type getType() const { return m_type; }
};

