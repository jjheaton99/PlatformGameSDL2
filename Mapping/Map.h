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
    Tile m_background{ "Assets/MapTiles/blackGrey.png", Tile::BACKGROUND };
    Tile m_block{ "Assets/MapTiles/WhiteFadeBlocks/1.png", Tile::SOLID };
    Tile m_platform{ "Assets/MapTiles/platform.png", Tile::PLATFORM };
    Tile m_ladder{ "Assets/MapTiles/ladder.png.", Tile::LADDER };

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
