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

    //pointer to global texture to avoid loading multiple copies of the same tile texture
    std::shared_ptr<WTexture> m_tileTexture;
    
public:
    //Tile(Type type = BACKGROUND, double xPos = 0, double yPos = 0, int size = Constants::tileSize);
    //Tile(int tileNumber, double xPos = 0, double yPos = 0, int size = Constants::tileSize);
    Tile(std::shared_ptr<WTexture> tileTexture = nullptr, Type type = BACKGROUND, double xPos = 0, double yPos = 0, int size = Constants::tileSize);

    void setPos(double xPos, double yPos) override;
    int getSize() const { return m_size; }
    Type getType() const { return m_type; }

    //void switchTileType(Type tileType);
    //void switchTileTypeFromInt(int tileType);

    void cameraDraw(const Camera& camera) const override;
};

