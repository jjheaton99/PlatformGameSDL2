#include "Map.h"

Map::Map()
{
    m_tileset->load("Assets/MapTiles/sciFiTiles.png");
}

Map::~Map()
{
    m_tileset->destroy();
}

void Map::update()
{
    /*int randomRow;
    int randomColumn;

    for (int i{ 0 }; i < 200; ++i)
    {
        randomRow = MTRandom::getRandomInt(0, m_map.size() - 1);
        randomColumn = MTRandom::getRandomInt(0, m_map[0].size() - 1);
        switchTile(randomRow, randomColumn, Tile::BACKGROUND);
    }*/
}

//generates a path out of map chunks
void Map::generateChunks(int totalChunks)
{
    int generatedChunksSize{ (2 * totalChunks) + 1 };
    m_generatedChunks.resize(generatedChunksSize);
    for (auto& row : m_generatedChunks)
    {
        row.resize(generatedChunksSize, MapChunkLoader::SOLID);
    }

    int randomChunkNum{ MTRandom::getRandomInt(1, 4) };
    int currentChunkRow{ totalChunks + 1 };
    int currentChunkColumn{ totalChunks + 1 };
    MapChunkLoader::ChunkEntrances prevChunkExit{MapChunkLoader::T};

    //variables to log space that path uses
    int minRow{ totalChunks };
    int maxRow{ totalChunks + 2 };
    int minColumn{ totalChunks };
    int maxColumn{ totalChunks + 2 };
    switch (randomChunkNum)
    {
    case 1:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::T;
        --currentChunkRow;
        if (currentChunkRow <= minRow)
        {
            minRow = currentChunkRow - 1;
        }
        prevChunkExit = MapChunkLoader::T;
        break;

    case 2:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::B;
        ++currentChunkRow;
        if (currentChunkRow >= maxRow)
        {
            maxRow = currentChunkRow + 1;
        }
        prevChunkExit = MapChunkLoader::B;
        break;

    case 3:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::L;
        --currentChunkColumn;
        if (currentChunkColumn <= minColumn)
        {
            minColumn = currentChunkColumn - 1;
        }
        prevChunkExit = MapChunkLoader::L;
        break;

    case 4:
        m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::R;
        ++currentChunkColumn;
        if (currentChunkColumn >= maxColumn)
        {
            maxColumn = currentChunkColumn + 1;
        }
        prevChunkExit = MapChunkLoader::R;
        break;
    }

    char dirChange;
    if (MTRandom::getRandomInt(1, 2) == 1)
    {
        dirChange = 'l';
    }
    else
    {
        dirChange = 'r';
    }
    int dirChangeCount{ 0 };
    int dirChangeOrNot;

    for (int i{ 1 }; i < totalChunks - 1; ++i)
    {
        dirChangeOrNot = MTRandom::getRandomInt(1, 2);
        switch (prevChunkExit)
        {
        case MapChunkLoader::T: 
            switch (dirChangeOrNot)
            {
            case 1:
                switch (dirChange)
                {
                case 'l':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BL;
                    --currentChunkColumn;
                    if (currentChunkColumn <= minColumn)
                    {
                        minColumn = currentChunkColumn - 1;
                    }
                    prevChunkExit = MapChunkLoader::L;
                    break;

                case 'r':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BR;
                    ++currentChunkColumn;
                    if (currentChunkColumn >= maxColumn)
                    {
                        maxColumn = currentChunkColumn + 1;
                    }
                    prevChunkExit = MapChunkLoader::R;
                    break;
                }
                ++dirChangeCount;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TB;
                --currentChunkRow;
                if (currentChunkRow <= minRow)
                {
                    minRow = currentChunkRow - 1;
                }
                prevChunkExit = MapChunkLoader::T;
                break;
            }
            break;

        case MapChunkLoader::B:
            switch (dirChangeOrNot)
            {
            case 1:
                switch (dirChange)
                {
                case 'l':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TR;
                    ++currentChunkColumn;
                    if (currentChunkColumn >= maxColumn)
                    {
                        maxColumn = currentChunkColumn + 1;
                    }
                    prevChunkExit = MapChunkLoader::R;
                    break;

                case 'r':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TL;
                    --currentChunkColumn;
                    if (currentChunkColumn <= minColumn)
                    {
                        minColumn = currentChunkColumn - 1;
                    }
                    prevChunkExit = MapChunkLoader::L;
                    break;
                }
                ++dirChangeCount;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TB;
                ++currentChunkRow;
                if (currentChunkRow >= maxRow)
                {
                    maxRow = currentChunkRow + 1;
                }
                prevChunkExit = MapChunkLoader::B;
                break;
            }
            break;

        case MapChunkLoader::L:
            switch (dirChangeOrNot)
            {
            case 1:
                switch (dirChange)
                {
                case 'l':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BR;
                    ++currentChunkRow;
                    if (currentChunkRow >= maxRow)
                    {
                        maxRow = currentChunkRow + 1;
                    }
                    prevChunkExit = MapChunkLoader::B;
                    break;

                case 'r':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TR;
                    --currentChunkRow;
                    if (currentChunkRow <= minRow)
                    {
                        minRow = currentChunkRow - 1;
                    }
                    prevChunkExit = MapChunkLoader::T;
                    break;
                }
                ++dirChangeCount;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::LR;
                --currentChunkColumn;
                if (currentChunkColumn <= minColumn)
                {
                    minColumn = currentChunkColumn - 1;
                }
                prevChunkExit = MapChunkLoader::L;
                break;
            }
            break;

        case MapChunkLoader::R:
            switch (dirChangeOrNot)
            {
            case 1:
                switch (dirChange)
                {
                case 'l':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::TL;
                    --currentChunkRow;
                    if (currentChunkRow <= minRow)
                    {
                        minRow = currentChunkRow - 1;
                    }
                    prevChunkExit = MapChunkLoader::T;
                    break;

                case 'r':
                    m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::BL;
                    ++currentChunkRow;
                    if (currentChunkRow >= maxRow)
                    {
                        maxRow = currentChunkRow + 1;
                    }
                    prevChunkExit = MapChunkLoader::B;
                    break;
                }
                ++dirChangeCount;
                break;

            case 2:
                m_generatedChunks[currentChunkRow][currentChunkColumn] = MapChunkLoader::LR;
                ++currentChunkColumn;
                if (currentChunkColumn >= maxColumn)
                {
                    maxColumn = currentChunkColumn + 1;
                }
                prevChunkExit = MapChunkLoader::R;
                break;
            }
            break;
        }

        //force direction to change same way two times
        if (dirChangeCount == 2)
        {
            if (dirChange == 'l')
            {
                dirChange = 'r';
            }
            else
            {
                dirChange = 'l';
            }
            dirChangeCount = 0;
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

    //erase excess solids leaving a border of solids
    if (minRow >= 0)
    {
        m_generatedChunks.erase(m_generatedChunks.begin(), m_generatedChunks.begin() + minRow);
        maxRow -= minRow;
    }
    if (maxRow < static_cast<int>(m_generatedChunks.size()))
    {
        m_generatedChunks.erase(m_generatedChunks.begin() + maxRow + 1, m_generatedChunks.end());
    }
    if (minColumn >= 0)
    {
        for (auto& row : m_generatedChunks)
        {
            row.erase(row.begin(), row.begin() + minColumn);
        }
        maxColumn -= minColumn;
    }
    if (maxColumn < static_cast<int>(m_generatedChunks[0].size()))
    {
        for (auto& row : m_generatedChunks)
        {
            row.erase(row.begin() + maxColumn + 1, row.end());
        }
    }

    m_playerSpawnChunk = Vector2D<int>{totalChunks + 1 - minColumn, totalChunks + 1 - minRow};
}

Tile Map::getTileFromNumber(int number) const
{
    switch (number)
    {
    case 0:
        return Tile{ m_tileset, 32 * number, 0, Tile::BACKGROUND };
    case 1:
        return Tile{ m_tileset, 32 * number, 0, Tile::BACKGROUND };
    case 2:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 3:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 4:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 5:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 6:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 7:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 8:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 9:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 10:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 11:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 12:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 13:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 14:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 15:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 16:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 17:
        return Tile{ m_tileset, 32 * number, 0, Tile::PLATFORM };
    case 18:
        return Tile{ m_tileset, 32 * number, 0, Tile::PLATFORM };
    case 19:
        return Tile{ m_tileset, 32 * number, 0, Tile::PLATFORM };
    case 20:
        return Tile{ m_tileset, 32 * number, 0, Tile::LADDER };
    case 21:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 22:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 23:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 24:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 25:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    case 26:
        return Tile{ m_tileset, 32 * number, 0, Tile::SOLID };
    default:
        return Tile{ m_tileset, 0, 0, Tile::BACKGROUND };
    }
}

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
                    m_map[row][column] = getTileFromNumber( tileNumbers[chunkRow][chunkColumn] );
                }
            }
        }
    }

    //level height and width used for determining camera boundary
    m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0].getSize();
    m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0].getSize();

    setTiles();
}

//loads tilemap from csv file of rows of numbers that correspond to tiles
void Map::loadMap(const char* fileName)
{
    //Erase any previous map
    m_map.clear();
    std::ifstream mapFile(fileName);
    if (!mapFile.is_open())
    {
        std::cout << "Unable to open file " << fileName << "!\n";
        return;
    }
    else
    {
        std::string line;
        std::vector<Tile> tileRow;
        //reads in file line by line and constructs tilemap row vector
        while (!mapFile.eof())
        {
            std::getline(mapFile, line);
            tileRow.resize(0);

            std::string tileNumberString{};
            int tileNumber{ 0 };
            int index{ 0 };
            int commaPos{ static_cast<int>(line.find(',', index)) };
            //finds all substrings sided by spaces and converts them integers to for pushing tiles
            while (commaPos != std::string::npos)
            {
                tileNumberString = line.substr(index, commaPos - index);
                index = commaPos + 1;
                commaPos = static_cast<int>(line.find(',', index));
                tileNumber = std::stoi(tileNumberString);
                tileRow.push_back(getTileFromNumber(tileNumber));
            }
            //last number in the line
            tileNumberString = line.substr(index, line.length() - index);
            tileRow.push_back(getTileFromNumber(tileNumber));
            //adds the row to the overall map
            m_map.push_back(tileRow);
        }
        mapFile.close();

        m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0].getSize();
        m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0].getSize();

        setTiles();
    }
}

//sets the positions of all the tiles
void Map::setTiles()
{
    for (int row{ 0 }; row < static_cast<int>(m_map.size()); ++row)
    {
        for (int column{ 0 }; column < static_cast<int>(m_map[0].size()); ++column)
        {
            m_map[row][column].setPos(1.0 * column * Constants::tileSize,
                1.0 * row * Constants::tileSize);
        }
    }
}

void Map::drawMap(const Camera& camera) const
{
    int xmaxCameraIndex{ Tile::coordToMapIndex(camera.getx() + camera.getw()) };
    int ymaxCameraIndex{ Tile::coordToMapIndex(camera.gety() + camera.geth()) };
    //for loop statements ensure only tiles in focus of camera are drawn
    for (int row{ Tile::coordToMapIndex(camera.gety()) };
        row < (ymaxCameraIndex + 1 < static_cast<int>(m_map.size()) ? ymaxCameraIndex + 1: static_cast<int>(m_map.size())); ++row)
    {
        for (int column{ Tile::coordToMapIndex(camera.getx()) };
            column < (xmaxCameraIndex + 1 < static_cast<int>(m_map[0].size()) ? xmaxCameraIndex + 1: static_cast<int>(m_map[0].size())); ++column)
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
