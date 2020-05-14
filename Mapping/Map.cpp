#include "Map.h"

Map::Map()
    : m_background{ "Assets/blackGrey.png", Tile::BACKGROUND },
    m_block{ "Assets/WhiteFadeBlocks/1.png", Tile::SOLID },
    m_platform{ "Assets/platform.png", Tile::PLATFORM },
    m_ladder{ "Assets/ladder.png.", Tile::LADDER }
{
}

Map::~Map()
{
    m_background.destroy();
    m_block.destroy();
}

//Selects tile based on number and pushes to temp vector
void Map::pushTile(int tileNumber, std::vector<Tile>& tileRow)
{
    switch (tileNumber)
    {
    case 0:
        tileRow.push_back(m_background);
        break;
    case 1:
        tileRow.push_back(m_block);
        break;
    case 2:
        tileRow.push_back(m_platform);
        break;
    case 3:
        tileRow.push_back(m_ladder);
        break;
    default:
        break;
    }
}

//loads tilemap from text file of rows of numbers that correspond to tile positions
//eg
//00 01 00 00 00
//00 01 00 00 00
//02 02 03 04 05
//05 04 03 02 01
bool Map::loadMap(const char* fileName)
{
    //Erase any previous map
    m_map.clear();

    std::ifstream mapFile(fileName);
    if (!mapFile.is_open())
    {
        std::cout << "Unable to open file " << fileName << "!\n";
        return false;
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
            index_type index{ 0 };
            index_type spacePos{ line.find(' ', index) };

            //finds all substrings sided by spaces and converts them integers to for pushing tiles
            while (spacePos != std::string::npos)
            {
                tileNumberString = line.substr(index, spacePos - index);
                index = spacePos + 1;
                spacePos = line.find(' ', index);

                tileNumber = std::stoi(tileNumberString);
                pushTile(tileNumber, tileRow);
            }

            //last number in the line
            tileNumberString = line.substr(index, line.length() - index);
            pushTile(tileNumber, tileRow);

            //adds the row to the overall map
            m_map.push_back(tileRow);
        }

        mapFile.close();

        //level height and width used for determining camera boundary
        m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0].getSize();
        m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0].getSize();

        setTiles();

        return true;
    }
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
