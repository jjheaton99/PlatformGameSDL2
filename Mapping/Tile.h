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
        LADDER,
        SPIKES
    };

private:
    Type m_type;
    int m_size;

    //pointer to global texture to avoid loading multiple copies of the same tile texture
    std::shared_ptr<WTexture> m_tileTexture;
    
public:
    Tile(std::shared_ptr<WTexture> tileTexture = nullptr, int srcRectx = 0, int srcRecty = 0, Type type = BACKGROUND, double xPos = 0, double yPos = 0, int size = Constants::tileSize);

    void setPos(double xPos, double yPos) override;
    int getSize() const { return m_size; }
    Type getType() const { return m_type; }

    void switchTileTypeAndTexture(std::shared_ptr<WTexture> tileTexture, Type tileType);

    static int coordToMapIndex(int coord);
    static int coordToMapIndex(double coord);

    void cameraDraw(const Camera& camera) const override;
};

