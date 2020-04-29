#pragma once

#include "Game.h"
#include "TextureW.h"
#include "GameObject.h"
#include "Tile.h"
#include <vector>

class Map
{
public:
    using map_type = std::vector<std::vector < int>>;
    using index_type = map_type::size_type;

private:
    SDL_Rect m_srcRect, m_destRect;
    TextureW m_dirt;
    TextureW m_grass;
    TextureW m_water;

    map_type m_map;

public:
    Map();
    ~Map();

    void loadMap(map_type map);
    void drawMap();
};
