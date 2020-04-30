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
    Tile m_background{"Assets/background.png", 0, 0, Tile::BACKGROUND};
    Tile m_block{ "Assets/block.png", 0, 0, Tile::SOLID };

public:
    Map(layout_type map);
    ~Map();

    void loadMap(layout_type map);
    void drawMap();

    std::vector<std::vector <Tile>>& getMap() { return m_map; }
};
