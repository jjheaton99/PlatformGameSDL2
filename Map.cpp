#include "Map.h"

Map::Map(layout_type map)
{
    loadMap(map);
}

Map::~Map()
{
    for (index_type row{ 0 }; row < m_map.size(); ++row)
    {
        for (index_type column{ 0 }; column < m_map[0].size(); ++column)
        {
            m_map[row][column].destroy();
        }
    }
}

void Map::loadMap(layout_type map)
{
    for (index_type row{ 0 }; row < map.size(); ++row)
    {
        m_map.push_back(std::vector<Tile>());
        for (index_type column{ 0 }; column < map[0].size(); ++column)
        {
            switch (map[row][column])
            {
            case 0:
                m_map[row].push_back(m_background);
                break;
            case 1:
                m_map[row].push_back(m_block);
                break;
            default:
                break;
            }
        }
    }
}

void Map::drawMap()
{
    for (index_type row{ 0 }; row < m_map.size(); ++row)
    {
        for (index_type column{ 0 }; column < m_map[0].size(); ++column)
        {
            m_map[row][column].setPos(column * static_cast<index_type>(m_map[row][column].getSize()), 
                row * static_cast<index_type>(m_map[row][column].getSize()));
            m_map[row][column].draw();
        }
    }
}