#include "MapChunkLoader.h"

MapChunkLoader::MapChunkLoader()
{}

bool MapChunkLoader::readFileToChunkVector(const std::string& file)
{
    m_chunkVector.clear();

    std::ifstream chunkFile(file);
    if (!chunkFile.is_open())
    {
        std::cout << "Unable to open file " << file << "!\n";
        return false;
    }

    else
    {
        std::string line;
        std::vector<int> tileRow;

        //reads in file line by line and constructs tilemap row vector
        while (!chunkFile.eof())
        {
            std::getline(chunkFile, line);
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
                tileRow.push_back(tileNumber);
            }

            //last number in the line
            tileNumberString = line.substr(index, line.length() - index);
            tileNumber = std::stoi(tileNumberString);
            tileRow.push_back(tileNumber);

            //adds the row to the overall chunk
            m_chunkVector.push_back(tileRow);
        }

        chunkFile.close();

        return true;
    }
}

const MapChunkLoader::intMap_type& MapChunkLoader::loadAndGetChunk(ChunkEntrances entrances)
{
    int index{};
	switch (entrances)
	{
	case MapChunkLoader::T:
        //index = MTRandom::getRandomInt(0, static_cast<int>(m_Tfiles.size()) - 1);
        readFileToChunkVector(m_Tfiles[0]);
        break;
	case MapChunkLoader::B:
        //index = MTRandom::getRandomInt(0, static_cast<int>(m_Bfiles.size()) - 1);
        readFileToChunkVector(m_Bfiles[0]);
        break;
	case MapChunkLoader::L:
        //index = MTRandom::getRandomInt(0, static_cast<int>(m_Lfiles.size()) - 1);
        readFileToChunkVector(m_Lfiles[0]);
        break;
	case MapChunkLoader::R:
        //index = MTRandom::getRandomInt(0, static_cast<int>(m_Rfiles.size()) - 1);
        readFileToChunkVector(m_Rfiles[0]);
        break;
	case MapChunkLoader::TB:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TBfiles.size()) - 1);
        readFileToChunkVector(m_TBfiles[index]);
        break;
	case MapChunkLoader::TL:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TLfiles.size()) - 1);
        readFileToChunkVector(m_TLfiles[index]);
        break;
	case MapChunkLoader::TR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TRfiles.size()) - 1);
        readFileToChunkVector(m_TRfiles[index]);
        break;
	case MapChunkLoader::BL:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_BLfiles.size()) - 1);
        readFileToChunkVector(m_BLfiles[index]);
        break;
	case MapChunkLoader::BR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_BRfiles.size()) - 1);
        readFileToChunkVector(m_BRfiles[index]);
        break;
	case MapChunkLoader::LR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_LRfiles.size()) - 1);
        readFileToChunkVector(m_LRfiles[index]);
        break;
	/*case MapChunkLoader::TBL:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TBLfiles.size()) - 1);
        readFileToChunkVector(m_TBLfiles[index]);
        break;
    case MapChunkLoader::TBR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TBRfiles.size()) - 1);
        readFileToChunkVector(m_TBRfiles[index]);
        break;
	case MapChunkLoader::TLR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TLRfiles.size()) - 1);
        readFileToChunkVector(m_TLRfiles[index]);
        break;
	case MapChunkLoader::BLR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_BLRfiles.size()) - 1);
        readFileToChunkVector(m_BLRfiles[index]);
        break;
	case MapChunkLoader::TBLR:
        index = MTRandom::getRandomInt(0, static_cast<int>(m_TBLRfiles.size()) - 1);
        readFileToChunkVector(m_TBLRfiles[index]);
        break;*/
    case MapChunkLoader::SOLID:
        readFileToChunkVector("Assets/Maps/MapChunks/solid.csv");
        break;
	}
    return m_chunkVector;
}

//for testing
void MapChunkLoader::printChunkVector() const
{
    for (const auto& row : m_chunkVector)
    {
        for (int element : row)
        {
            std::cout << element << ' ';
        }
        std::cout << '\n';
    }
}