#pragma once

#include "Tile.h"
#include <array>
#include <vector>

class Map
{
public:
    using map_type = std::vector<std::vector <Tile>>;
    using layout_type = std::array<std::array<int, 32>, 18>;
    using index_type = map_type::size_type;

private:
    map_type m_map;
    Tile m_background{"Assets/background2.png", 0, 0, Tile::BACKGROUND};
    Tile m_block1{ "Assets/WhiteFadeBlocks/1.png", 0, 0, Tile::SOLID };
    Tile m_block2{ "Assets/WhiteFadeBlocks/2.png", 0, 0, Tile::SOLID };
    Tile m_block3{ "Assets/WhiteFadeBlocks/3.png", 0, 0, Tile::SOLID };
    Tile m_block4{ "Assets/WhiteFadeBlocks/4.png", 0, 0, Tile::SOLID };
    Tile m_block5{ "Assets/WhiteFadeBlocks/5.png", 0, 0, Tile::SOLID };
    Tile m_block6{ "Assets/WhiteFadeBlocks/6.png", 0, 0, Tile::SOLID };
    Tile m_block7{ "Assets/WhiteFadeBlocks/7.png", 0, 0, Tile::SOLID };
    Tile m_block8{ "Assets/WhiteFadeBlocks/8.png", 0, 0, Tile::SOLID };
    Tile m_block9{ "Assets/WhiteFadeBlocks/9.png", 0, 0, Tile::SOLID };
    Tile m_block10{ "Assets/WhiteFadeBlocks/10.png", 0, 0, Tile::SOLID };
    Tile m_block11{ "Assets/WhiteFadeBlocks/11.png", 0, 0, Tile::SOLID };
    Tile m_block12{ "Assets/WhiteFadeBlocks/12.png", 0, 0, Tile::SOLID };
    Tile m_block13{ "Assets/WhiteFadeBlocks/13.png", 0, 0, Tile::SOLID };
    Tile m_block14{ "Assets/WhiteFadeBlocks/14.png", 0, 0, Tile::SOLID };
    Tile m_block15{ "Assets/WhiteFadeBlocks/15.png", 0, 0, Tile::SOLID };
    Tile m_block16{ "Assets/WhiteFadeBlocks/16.png", 0, 0, Tile::SOLID };

public:
    Map(layout_type map);
    ~Map();

    void loadMap(layout_type map);
    void drawMap();

    std::vector<std::vector <Tile>>& getMap() { return m_map; }
};
