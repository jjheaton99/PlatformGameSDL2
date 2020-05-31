#include "Map.h"

Map::Map()
{}

Map::~Map()
{
    for (auto& row : m_map)
    {
        for (auto& tile : row)
        {
            tile.destroy();
        }
    }

    m_background.destroy();
    m_block.destroy();
    m_platform.destroy();
    m_ladder.destroy();

    m_generatedChunks.clear();
    m_map.clear();
}

void Map::generateChunks(int totalChunks)
{
    m_generatedChunks.resize(3);
    for (auto& row : m_generatedChunks)
    {
        row.resize(3, MapChunkLoader::ChunkEntrances::TBLR);
    }

    //m_generatedChunks[0][0] = MapChunkLoader::ChunkEntrances::BR;
}

//Selects tile based on number and pushes to temp vector
Tile Map::addTile(int tileNumber)
{
    switch (tileNumber)
    {
    case 0:
        return m_background;
    case 1:
        return m_block;
    case 2:
        return m_platform;
    case 3:
        return m_ladder;
    default:
        return m_background;
    }
}

//loads tilemap from text file of rows of numbers that correspond to tile positions
//eg
//00 01 00 00 00
//00 01 00 00 00
//02 02 03 04 05
//05 04 03 02 01
void Map::loadMap(int totalChunks)
{
    //Resize map based on generated chunks and size of chunks
    generateChunks(totalChunks);
    m_map.resize(m_chunkHeight * m_generatedChunks.size());
    for (auto& row : m_map)
    {
        row.resize(m_chunkWidth * m_generatedChunks[0].size());
    }

    //for loops to load tiles into map
    //loops over the generated chunks and and loads a random chunk of that type into the map
    for (int generatedChunksRow{ 0 }; generatedChunksRow < static_cast<int>(m_generatedChunks.size()); ++generatedChunksRow)
    {
        for (int generatedChunksColumn{ 0 }; generatedChunksColumn < static_cast<int>(m_generatedChunks[0].size()); ++generatedChunksColumn)
        {
            MapChunkLoader::intMap_type tileNumbers{ m_chunkLoader.loadAndGetChunk(m_generatedChunks[generatedChunksRow][generatedChunksColumn]) };
            for (int chunkRow{ 0 }; chunkRow < m_chunkHeight; ++chunkRow)
            {
                for (int chunkColumn{ 0 }; chunkColumn < m_chunkWidth; ++chunkColumn)
                {
                    m_map[(generatedChunksRow * m_chunkHeight) + chunkRow][(generatedChunksColumn * m_chunkWidth) + chunkColumn] = addTile(tileNumbers[chunkRow][chunkColumn]);
                }
            }
        }
    }

    //level height and width used for determining camera boundary
    m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0].getSize();
    m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0].getSize();

    setTiles();

    printMap();
}

//sets the positions of all the tiles
void Map::setTiles()
{
    for (index_type row{ 0 }; row < m_map.size(); ++row)
    {
        for (index_type column{ 0 }; column < m_map[0].size(); ++column)
        {
            m_map[row][column].setPos(1.0 * column * m_map[row][column].getSize(),
                1.0 * row * m_map[row][column].getSize());
        }
    }
}

Map::index_type Map::cameraCoordToMapIndex(int coord) const
{
    return (coord - (coord % m_map[0][0].getSize())) / m_map[0][0].getSize();
}

void Map::drawMap(const Camera& camera) const
{
    index_type xmaxCameraIndex{ cameraCoordToMapIndex(camera.getx() + camera.getw()) };
    index_type ymaxCameraIndex{ cameraCoordToMapIndex(camera.gety() + camera.geth()) };
    //for loop statements ensure only tiles in focus of camera are drawn
    for (index_type row{ cameraCoordToMapIndex(camera.gety()) };
        row < (ymaxCameraIndex + 1 < m_map.size() ? ymaxCameraIndex + 1: m_map.size()); ++row)
    {
        for (index_type column{ cameraCoordToMapIndex(camera.getx()) };
            column < (xmaxCameraIndex + 1 < m_map[0].size() ? xmaxCameraIndex + 1: m_map[0].size()); ++column)
        {
            m_map[row][column].cameraDraw(camera);
        }
    }
}

void Map::printMap() const
{
    for (const auto& row : m_map)
    {
        for (const auto& tile : row)
        {
            switch (tile.getType())
            {
            case Tile::BACKGROUND:
                std::cout << "0 ";
                break;
            case Tile::SOLID:
                std::cout << "1 ";
                break;
            case Tile::PLATFORM:
                std::cout << "2 ";
                break;
            case Tile::LADDER:
                std::cout << "3 ";
                break;
            default:
                std::cout << "x ";
                break;
            }
        }
        std::cout << '\n';
    }
}