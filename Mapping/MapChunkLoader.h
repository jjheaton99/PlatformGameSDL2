#pragma once

#include "Random.h"
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

    std::vector<std::string> m_Tfiles{ 
        "Assets/Maps/MapChunks/T/a35.csv" 
    };
    std::vector<std::string> m_Bfiles{ 
        "Assets/Maps/MapChunks/B/a35.csv" 
    };
    std::vector<std::string> m_Lfiles{ 
        "Assets/Maps/MapChunks/L/a35.csv" 
    };
    std::vector<std::string> m_Rfiles{ 
        "Assets/Maps/MapChunks/R/a35.csv" 
    };
    std::vector<std::string> m_TBfiles{ 
        "Assets/Maps/MapChunks/TB/a35.csv",
        "Assets/Maps/MapChunks/TB/b35.csv",
        "Assets/Maps/MapChunks/TB/c35.csv"
    };
    std::vector<std::string> m_TLfiles{ 
        "Assets/Maps/MapChunks/TL/a35.csv",
        "Assets/Maps/MapChunks/TL/b35.csv",
        "Assets/Maps/MapChunks/TL/c35.csv"
    };
    std::vector<std::string> m_TRfiles{
        "Assets/Maps/MapChunks/TR/a35.csv",
        "Assets/Maps/MapChunks/TR/b35.csv",
        "Assets/Maps/MapChunks/TR/c35.csv"
    };
    std::vector<std::string> m_BLfiles{
        "Assets/Maps/MapChunks/BL/a35.csv",
        "Assets/Maps/MapChunks/BL/b35.csv",
        "Assets/Maps/MapChunks/BL/c35.csv"
    };
    std::vector<std::string> m_BRfiles{
        "Assets/Maps/MapChunks/BR/a35.csv",
        "Assets/Maps/MapChunks/BR/b35.csv",
        "Assets/Maps/MapChunks/BR/c35.csv"
    };
    std::vector<std::string> m_LRfiles{ 
        "Assets/Maps/MapChunks/LR/a35.csv",
        "Assets/Maps/MapChunks/LR/b35.csv",
        "Assets/Maps/MapChunks/LR/c35.csv"
    };
    /*std::vector<std::string> m_TBLfiles{};
    std::vector<std::string> m_TBRfiles{};
    std::vector<std::string> m_TLRfiles{};
    std::vector<std::string> m_BLRfiles{};
    std::vector<std::string> m_TBLRfiles{};*/

    bool readFileToChunkVector(const std::string& file);

public:
    MapChunkLoader();

    const intMap_type& loadAndGetChunk(ChunkEntrances entrances);
    
    void printChunkVector() const;
};

