#include "Map.h"

Map::Map()
{}

Map::~Map()
{
    for (auto& row : m_map)
    {
        for (auto& tile : row)
        {
            tile->destroy();
        }
    }
}

void Map::generateChunks(int totalChunks)
{
    m_generatedChunks.resize((2 * totalChunks) - 1);
    for (auto& row : m_generatedChunks)
    {
        row.resize((2 * totalChunks) - 1, MapChunkLoader::SOLID);
    }

    int randomChunkNum{ MTRandom::getRandomInt(1, 4) };
    int currentChunkRow{ totalChunks - 1 };
    int currentChunkColumn{ totalChunks - 1 };
    MapChunkLoader::ChunkEntrances prevChunkExit;

    switch (randomChunkNum)
    {
    case 1:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::T;
        --currentChunkRow;
        prevChunkExit = MapChunkLoader::T;
        break;

    case 2:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::B;
        ++currentChunkRow;
        prevChunkExit = MapChunkLoader::B;
        break;

    case 3:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::L;
        --currentChunkColumn;
        prevChunkExit = MapChunkLoader::L;
        break;

    case 4:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::R;
        ++currentChunkColumn;
        prevChunkExit = MapChunkLoader::R;
        break;
    }

    for (int i{ 1 }; i < totalChunks - 1; ++i)
    {
        randomChunkNum = MTRandom::getRandomInt(1, 3);
        switch (prevChunkExit)
        {
        case MapChunkLoader::T:
            switch (randomChunkNum)
            {
            case 1:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BL;
                --currentChunkColumn;
                prevChunkExit = MapChunkLoader::L;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TB;
                --currentChunkRow;
                prevChunkExit = MapChunkLoader::T;
                break;

            case 3:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BR;
                ++currentChunkColumn;
                prevChunkExit = MapChunkLoader::R;
                break;
            }
            break;

        case MapChunkLoader::B:
            switch (randomChunkNum)
            {
            case 1:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TB;
                ++currentChunkRow;
                prevChunkExit = MapChunkLoader::B;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TL;
                --currentChunkColumn;
                prevChunkExit = MapChunkLoader::L;
                break;

            case 3:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TR;
                ++currentChunkColumn;
                prevChunkExit = MapChunkLoader::R;
                break;
            }
            break;

        case MapChunkLoader::L:
            switch (randomChunkNum)
            {
            case 1:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TR;
                --currentChunkRow;
                prevChunkExit = MapChunkLoader::T;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BR;
                ++currentChunkRow;
                prevChunkExit = MapChunkLoader::B;
                break;

            case 3:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::LR;
                --currentChunkColumn;
                prevChunkExit = MapChunkLoader::L;
                break;
            }
            break;

        case MapChunkLoader::R:
            switch (randomChunkNum)
            {
            case 1:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TL;
                --currentChunkRow;
                prevChunkExit = MapChunkLoader::T;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BL;
                ++currentChunkRow;
                prevChunkExit = MapChunkLoader::B;
                break;

            case 3:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::LR;
                ++currentChunkColumn;
                prevChunkExit = MapChunkLoader::R;
                break;
            }
            break;
        }
    }

    switch (prevChunkExit)
    {
    case MapChunkLoader::T:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::B;
        break;

    case MapChunkLoader::B:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::T;
        break;

    case MapChunkLoader::L:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::R;
        break;

    case MapChunkLoader::R:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::L;
        break;
    }

    for (const auto& row : m_generatedChunks)
    {
        for (const auto& chunk : row)
        {
            std::cout << static_cast<int>(chunk) << "   ";
        }
        std::cout << '\n';
    }
}

//Selects tile based on number and pushes to temp vector
Tile::Type Map::getTileTypeFromNumber(int tileNumber) const
{
    switch (tileNumber)
    {
    case 0:
        return Tile::BACKGROUND;
    case 1:
        return Tile::SOLID;
    case 2:
        return Tile::PLATFORM;
    case 3:
        return Tile::LADDER;
    default:
        return Tile::BACKGROUND;
    }
}

std::string Map::getTileFileFromNumber(int tileNumber) const
{
    switch (tileNumber)
    {
    case 0:
        return "Assets/MapTiles/blackGrey.png";
    case 1:
        return "Assets/MapTiles/WhiteFadeBlocks/1.png";
    case 2:
        return "Assets/MapTiles/platform.png";
    case 3:
        return "Assets/MapTiles/ladder.png.";
    default:
        return "Assets/MapTiles/blackGrey.png";
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
    m_map.resize(Constants::chunkHeight * m_generatedChunks.size());
    for (auto& row : m_map)
    {
        row.resize(Constants::chunkWidth * m_generatedChunks[0].size());
    }

    //for loops to load tiles into map
    //loops over the generated chunks and and loads a random chunk of that type into the map
    for (int generatedChunksRow{ 0 }; generatedChunksRow < static_cast<int>(m_generatedChunks.size()); ++generatedChunksRow)
    {
        for (int generatedChunksColumn{ 0 }; generatedChunksColumn < static_cast<int>(m_generatedChunks[0].size()); ++generatedChunksColumn)
        {
            MapChunkLoader::intMap_type tileNumbers{ m_chunkLoader.loadAndGetChunk(m_generatedChunks[generatedChunksRow][generatedChunksColumn]) };
            for (int chunkRow{ 0 }; chunkRow < Constants::chunkHeight; ++chunkRow)
            {
                int row{ (generatedChunksRow * Constants::chunkHeight) + chunkRow };
                for (int chunkColumn{ 0 }; chunkColumn < Constants::chunkWidth; ++chunkColumn)
                {
                    int column{ (generatedChunksColumn * Constants::chunkWidth) + chunkColumn };
                    m_map[row][column].reset(new Tile{ tileNumbers[chunkRow][chunkColumn] });
                }
            }
        }
    }

    //level height and width used for determining camera boundary
    m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0]->getSize();
    m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0]->getSize();

    setTiles();

    //printMap();
}

//sets the positions of all the tiles
void Map::setTiles()
{
    for (index_type row{ 0 }; row < m_map.size(); ++row)
    {
        for (index_type column{ 0 }; column < m_map[0].size(); ++column)
        {
            m_map[row][column]->setPos(1.0 * column * Constants::tileSize,
                1.0 * row * Constants::tileSize);
        }
    }
}

Map::index_type Map::cameraCoordToMapIndex(int coord) const
{
    return (coord - (coord % m_map[0][0]->getSize())) / m_map[0][0]->getSize();
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
            m_map[row][column]->cameraDraw(camera);
        }
    }
}

void Map::printMap() const
{
    for (const auto& row : m_map)
    {
        for (const auto& tile : row)
        {
            switch (tile->getType())
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