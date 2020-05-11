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

    int m_levelWidth;
    int m_levelHeight;
   
    void pushTile(int tileNumber, std::vector<Tile>& tileRow);
    index_type cameraCoordToMapIndex(int coord);
    void setTiles();

public:
    Map();
    ~Map();

    bool loadMap(const char* fileName);
    //void loadMap(layout_type map);
    void drawMap(Camera& camera);

    std::vector<std::vector <Tile>>& getMap() { return m_map; }
    int getLevelWidth() { return m_levelWidth; }
    int getLevelHeight() { return m_levelHeight; }
};

/*
Tile m_block2;
Tile m_block3;
Tile m_block4;
Tile m_block5;
Tile m_block6;
Tile m_block7;
Tile m_block8;
Tile m_block9;
Tile m_block10;
Tile m_block11;
Tile m_block12;
Tile m_block13;
Tile m_block14;
Tile m_block15;
Tile m_block16;
*/