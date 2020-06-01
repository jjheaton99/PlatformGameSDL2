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
            int spacePos{ static_cast<int>(line.find(' ', index)) };

            //finds all substrings sided by spaces and converts them integers to for pushing tiles
            while (spacePos != std::string::npos)
            {
                tileNumberString = line.substr(index, spacePos - index);
                index = spacePos + 1;
                spacePos = line.find(' ', index);

                tileNumber = std::stoi(tileNumberString);
                tileRow.push_back(tileNumber);
            }

            //last number in the line
            tileNumberString = line.substr(index, line.length() - index);
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
	switch (entrances)
	{
	case MapChunkLoader::T:
        readFileToChunkVector(m_Tfiles[0]);
        break;
	case MapChunkLoader::B:
        readFileToChunkVector(m_Bfiles[0]);
        break;
	case MapChunkLoader::L:
        readFileToChunkVector(m_Lfiles[0]);
        break;
	case MapChunkLoader::R:
        readFileToChunkVector(m_Rfiles[0]);
        break;
	case MapChunkLoader::TB:
        readFileToChunkVector(m_TBfiles[0]);
        break;
	case MapChunkLoader::TL:
        readFileToChunkVector(m_TLfiles[0]);
        break;
	case MapChunkLoader::TR:
        readFileToChunkVector(m_TRfiles[0]);
        break;
	case MapChunkLoader::BL:
        readFileToChunkVector(m_BLfiles[0]);
        break;
	case MapChunkLoader::BR:
        readFileToChunkVector(m_BRfiles[0]);
        break;
	case MapChunkLoader::LR:
        readFileToChunkVector(m_LRfiles[0]);
        break;
	case MapChunkLoader::TBL:
        readFileToChunkVector(m_TBLfiles[0]);
        break;
	case MapChunkLoader::TBR:
        readFileToChunkVector(m_TBRfiles[0]);
        break;
	case MapChunkLoader::TLR:
        readFileToChunkVector(m_TLRfiles[0]);
        break;
	case MapChunkLoader::BLR:
        readFileToChunkVector(m_BLRfiles[0]);
        break;
	case MapChunkLoader::TBLR:
        readFileToChunkVector(m_TBLRfiles[0]);
        break;
    case MapChunkLoader::SOLID:
        readFileToChunkVector("Assets/Maps/MapChunks/solid.txt");
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