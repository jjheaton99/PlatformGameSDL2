#include "Map.h"

Map::Map()
    : m_background{ "Assets/blackGrey.png", Tile::BACKGROUND },
    m_block{ "Assets/WhiteFadeBlocks/1.png", Tile::SOLID }
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

    default:
        break;
    }
}

//Loads tilemap from text file. Returns false if map wasn't loaded
bool Map::loadMap(const char* fileName)
{
    //Erase any previous map
    m_map.resize(0);

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

        while (!mapFile.eof())
        {
            std::getline(mapFile, line);
            tileRow.resize(0);
            
            std::string tileNumberString{};
            int tileNumber{ 0 };
            index_type index{ 0 };
            index_type spacePos{ line.find(' ', index) };

            while (spacePos != std::string::npos)
            {
                tileNumberString = line.substr(index, spacePos - index);
                index = spacePos + 1;
                spacePos = line.find(' ', index);

                tileNumber = std::stoi(tileNumberString);
                pushTile(tileNumber, tileRow);
            }

            tileNumberString = line.substr(index, line.length() - index);
            pushTile(tileNumber, tileRow);

            m_map.push_back(tileRow);
        }

        mapFile.close();

        m_levelHeight = static_cast<int>(m_map.size()) * m_map[0][0].getSize();
        m_levelWidth = static_cast<int>(m_map[0].size()) * m_map[0][0].getSize();

        return true;
    }
}

void Map::drawMap(Camera& camera)
{
    for (index_type row{ 0 }; row < m_map.size(); ++row)
    {
        for (index_type column{ 0 }; column < m_map[0].size(); ++column)
        {
            m_map[row][column].setPos(column * static_cast<int>(m_map[row][column].getSize()), 
                row * static_cast<int>(m_map[row][column].getSize()));
            m_map[row][column].cameraDraw(camera);
        }
    }
}

/*
m_block2{ "Assets/WhiteFadeBlocks/2.png", Tile::SOLID },
m_block3{ "Assets/WhiteFadeBlocks/3.png", Tile::SOLID },
m_block4{ "Assets/WhiteFadeBlocks/4.png", Tile::SOLID },
m_block5{ "Assets/WhiteFadeBlocks/5.png", Tile::SOLID },
m_block6{ "Assets/WhiteFadeBlocks/6.png", Tile::SOLID },
m_block7{ "Assets/WhiteFadeBlocks/7.png", Tile::SOLID },
m_block8{ "Assets/WhiteFadeBlocks/8.png", Tile::SOLID },
m_block9{ "Assets/WhiteFadeBlocks/9.png", Tile::SOLID },
m_block10{ "Assets/WhiteFadeBlocks/10.png", Tile::SOLID },
m_block11{ "Assets/WhiteFadeBlocks/11.png", Tile::SOLID },
m_block12{ "Assets/WhiteFadeBlocks/12.png", Tile::SOLID },
m_block13{ "Assets/WhiteFadeBlocks/13.png", Tile::SOLID },
m_block14{ "Assets/WhiteFadeBlocks/14.png", Tile::SOLID },
m_block15{ "Assets/WhiteFadeBlocks/15.png", Tile::SOLID },
m_block16{ "Assets/WhiteFadeBlocks/16.png", Tile::SOLID }
*/

/*
    m_block2.destroy();
    m_block3.destroy();
    m_block4.destroy();
    m_block5.destroy();
    m_block6.destroy();
    m_block7.destroy();
    m_block8.destroy();
    m_block9.destroy();
    m_block10.destroy();
    m_block11.destroy();
    m_block12.destroy();
    m_block13.destroy();
    m_block14.destroy();
    m_block15.destroy();
    m_block16.destroy();
*/

/*
    case 2:
        tileRow.push_back(m_block2);
        break;
    case 3:
        tileRow.push_back(m_block3);
        break;
    case 4:
        tileRow.push_back(m_block4);
        break;
    case 5:
        tileRow.push_back(m_block5);
        break;
    case 6:
        tileRow.push_back(m_block6);
        break;
    case 7:
        tileRow.push_back(m_block7);
        break;
    case 8:
        tileRow.push_back(m_block8);
        break;
    case 9:
        tileRow.push_back(m_block9);
        break;
    case 10:
        tileRow.push_back(m_block10);
        break;
    case 11:
        tileRow.push_back(m_block11);
        break;
    case 12:
        tileRow.push_back(m_block12);
        break;
    case 13:
        tileRow.push_back(m_block13);
        break;
    case 14:
        tileRow.push_back(m_block14);
        break;
    case 15:
        tileRow.push_back(m_block15);
        break;
    case 16:
        tileRow.push_back(m_block16);
        break;
        */