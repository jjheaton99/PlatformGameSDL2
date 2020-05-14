#pragma once

#include "Tile.h"
#include "Camera.h"
#include <array>
#include <vector>
#include <fstream>
#include <string>

//class for managing tilemapping
class Map
{
public:
    using map_type = std::vector<std::vector <Tile>>;
    using index_type = map_type::size_type;

private:
    map_type m_map;
    Tile m_background;
    Tile m_block;
    Tile m_platform;
    Tile m_ladder;

    int m_levelWidth;
    int m_levelHeight;
   
    void pushTile(int tileNumber, std::vector<Tile>& tileRow);
    index_type cameraCoordToMapIndex(int coord) const;
    void setTiles();

public:
    Map();
    ~Map();

    bool loadMap(const char* fileName);
    void drawMap(const Camera& camera) const;

    const std::vector<std::vector <Tile>>& getMap() const { return m_map; }
    int getLevelWidth() const { return m_levelWidth; }
    int getLevelHeight() const { return m_levelHeight; }
};
