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
                m_map[row].push_back(m_block1);
                break;
            case 2:
                m_map[row].push_back(m_block2);
                break;
            case 3:
                m_map[row].push_back(m_block3);
                break;
            case 4:
                m_map[row].push_back(m_block4);
                break;
            case 5:
                m_map[row].push_back(m_block5);
                break;
            case 6:
                m_map[row].push_back(m_block6);
                break;
            case 7:
                m_map[row].push_back(m_block7);
                break;
            case 8:
                m_map[row].push_back(m_block8);
                break;
            case 9:
                m_map[row].push_back(m_block9);
                break;
            case 10:
                m_map[row].push_back(m_block10);
                break;
            case 11:
                m_map[row].push_back(m_block11);
                break;
            case 12:
                m_map[row].push_back(m_block12);
                break;
            case 13:
                m_map[row].push_back(m_block13);
                break;
            case 14:
                m_map[row].push_back(m_block14);
                break;
            case 15:
                m_map[row].push_back(m_block15);
                break;
            case 16:
                m_map[row].push_back(m_block16);
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