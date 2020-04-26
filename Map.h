#pragma once

#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <array>

class Map
{
public:
    using map_type = std::array<std::array < int, 25>, 20>;
    using index_type = map_type::size_type;

private:
    SDL_Rect m_srcRect, m_destRect;
    SDL_Texture* m_dirt;
    SDL_Texture* m_grass;
    SDL_Texture* m_water;

    map_type m_map;

public:
    Map();
    ~Map();

    void loadMap(map_type map);
    void drawMap();
};
