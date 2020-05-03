#pragma once

#include "Tile.h"
#include <array>
#include <vector>
#include <fstream>
#include <string>

class Map
{
public:
    using map_type = std::vector<std::vector <Tile>>;
    //using layout_type = std::array<std::array<int, 32>, 18>;
    using index_type = map_type::size_type;

private:
    map_type m_map;
    Tile m_background;
    Tile m_block1;
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

    void pushTile(int tileNumber, std::vector<Tile>& tileRow);

public:
    Map();
    ~Map();

    bool loadMap(const char* fileName);
    //void loadMap(layout_type map);
    void drawMap();

    std::vector<std::vector <Tile>>& getMap() { return m_map; }
};
