#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <iostream>

class MapChunkLoader
{
public:
    //T = top, B = bottom, L = left, R = right
    enum ChunkEntrances
    {
        T,
        B,
        L,
        R,
        TB,
        TL,
        TR,
        BL,
        BR,
        LR,
        TBL,
        TBR,
        TLR,
        BLR,
        TBLR,
        SOLID
    };

    using intMap_type = std::vector<std::vector<int>>;

private:
    intMap_type m_chunkVector;

    std::vector<std::string> m_Tfiles{ "Assets/Maps/MapChunks/BL/a35.csv" };
    std::vector<std::string> m_Bfiles{ "Assets/Maps/MapChunks/BL/a35.csv" };
    std::vector<std::string> m_Lfiles{ "Assets/Maps/MapChunks/BL/a35.csv" };
    std::vector<std::string> m_Rfiles{ "Assets/Maps/MapChunks/BL/a35.csv" };
    std::vector<std::string> m_TBfiles{ "Assets/Maps/MapChunks/TBLR/a35.csv" };
    std::vector<std::string> m_TLfiles{ "Assets/Maps/MapChunks/TBLR/a35.csv" };
    std::vector<std::string> m_TRfiles{ "Assets/Maps/MapChunks/TBLR/a35.csv" };
    std::vector<std::string> m_BLfiles{ "Assets/Maps/MapChunks/BL/a35.csv" };
    std::vector<std::string> m_BRfiles{ "Assets/Maps/MapChunks/TBLR/a35.csv" };
    std::vector<std::string> m_LRfiles{ "Assets/Maps/MapChunks/TBLR/a35.csv" };
    std::vector<std::string> m_TBLfiles{};
    std::vector<std::string> m_TBRfiles{};
    std::vector<std::string> m_TLRfiles{};
    std::vector<std::string> m_BLRfiles{};
    std::vector<std::string> m_TBLRfiles{};

    bool readFileToChunkVector(const std::string& file);

public:
    MapChunkLoader();

    const intMap_type& loadAndGetChunk(ChunkEntrances entrances);
    
    void printChunkVector() const;
};

