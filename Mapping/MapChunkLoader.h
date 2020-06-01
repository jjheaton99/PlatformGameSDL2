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

    std::vector<std::string> m_Tfiles{ "Assets/Maps/MapChunks/T/standard.txt" };
    std::vector<std::string> m_Bfiles{ "Assets/Maps/MapChunks/B/standard.txt" };
    std::vector<std::string> m_Lfiles{ "Assets/Maps/MapChunks/L/standard.txt" };
    std::vector<std::string> m_Rfiles{ "Assets/Maps/MapChunks/R/standard.txt" };
    std::vector<std::string> m_TBfiles{ "Assets/Maps/MapChunks/TB/standard.txt" };
    std::vector<std::string> m_TLfiles{ "Assets/Maps/MapChunks/TL/standard.txt" };
    std::vector<std::string> m_TRfiles{ "Assets/Maps/MapChunks/TR/standard.txt" };
    std::vector<std::string> m_BLfiles{ "Assets/Maps/MapChunks/BL/standard.txt" };
    std::vector<std::string> m_BRfiles{ "Assets/Maps/MapChunks/BR/standard.txt" };
    std::vector<std::string> m_LRfiles{ "Assets/Maps/MapChunks/LR/standard.txt" };
    std::vector<std::string> m_TBLfiles{ "Assets/Maps/MapChunks/TBL/standard.txt" };
    std::vector<std::string> m_TBRfiles{ "Assets/Maps/MapChunks/TBR/standard.txt" };
    std::vector<std::string> m_TLRfiles{ "Assets/Maps/MapChunks/TLR/standard.txt" };
    std::vector<std::string> m_BLRfiles{ "Assets/Maps/MapChunks/BLR/standard.txt" };
    std::vector<std::string> m_TBLRfiles{ "Assets/Maps/MapChunks/TBLR/standard.txt" };

    bool readFileToChunkVector(const std::string& file);

public:
    MapChunkLoader();

    const intMap_type& loadAndGetChunk(ChunkEntrances entrances);
    
    void printChunkVector() const;
};

