#pragma once

#include "Random.h"
#include "Vector2D.h"
#include <utility>
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

    struct MapChunkData
    {
        intMap_type tiles;
        std::vector<Vector2D<double>> spawnPoints;
    };

private:
    MapChunkData m_chunkData;

    using chunkInfo = std::vector<std::pair<std::string, std::vector<Vector2D<double>>>>;

    //spawn points are hard coded for each map chunk
    chunkInfo m_Tfiles{
        {"Assets/Maps/MapChunks/T/a35.csv", {}}
    };
    chunkInfo m_Bfiles{
        {"Assets/Maps/MapChunks/B/a35.csv", {}}
    };
    chunkInfo m_Lfiles{
        {"Assets/Maps/MapChunks/L/a35.csv", {}}
    };
    chunkInfo m_Rfiles{
        {"Assets/Maps/MapChunks/R/a35.csv", {}}
    };
    chunkInfo m_TBfiles{
        {"Assets/Maps/MapChunks/TB/a35.csv", {
            Vector2D<double>{560, 700},
            Vector2D<double>{190, 75},
            Vector2D<double>{560, 400},
            Vector2D<double>{900, 75}
        }},
        {"Assets/Maps/MapChunks/TB/b35.csv", {
            Vector2D<double>{850, 180},
            Vector2D<double>{320, 360},
            Vector2D<double>{785, 500},
            Vector2D<double>{290, 680}
        }},
        {"Assets/Maps/MapChunks/TB/c35.csv", {
            Vector2D<double>{128, 244},
            Vector2D<double>{874, 246},
            Vector2D<double>{126, 757},
            Vector2D<double>{870, 773}
        }}
    };
    chunkInfo m_TLfiles{
        {"Assets/Maps/MapChunks/TL/a35.csv", {
            Vector2D<double>{280, 700},
            Vector2D<double>{650, 700},
            Vector2D<double>{500, 300},
            Vector2D<double>{800, 80}
        }},
        {"Assets/Maps/MapChunks/TL/b35.csv", {
            Vector2D<double>{220, 600},
            Vector2D<double>{850, 800},
            Vector2D<double>{370, 220},
            Vector2D<double>{690, 220}
        }},
        {"Assets/Maps/MapChunks/TL/c35.csv", {
            Vector2D<double>{360, 680},
            Vector2D<double>{770, 670},
            Vector2D<double>{500, 300},
            Vector2D<double>{230, 70}
        }}
    };
    chunkInfo m_TRfiles{
        {"Assets/Maps/MapChunks/TR/a35.csv", {
            Vector2D<double>{210, 670},
            Vector2D<double>{600, 660},
            Vector2D<double>{420, 300},
            Vector2D<double>{760, 70}
        }},
        {"Assets/Maps/MapChunks/TR/b35.csv", {
            Vector2D<double>{210, 780},
            Vector2D<double>{850, 600},
            Vector2D<double>{360, 240},
            Vector2D<double>{690, 240}
        }},
        {"Assets/Maps/MapChunks/TR/c35.csv", {
            Vector2D<double>{340, 690},
            Vector2D<double>{700, 690},
            Vector2D<double>{700, 300},
            Vector2D<double>{230, 70}
        }}
    };
    chunkInfo m_BLfiles{
        {"Assets/Maps/MapChunks/BL/a35.csv", {
            Vector2D<double>{500, 70},
            Vector2D<double>{230, 870},
            Vector2D<double>{820, 850},
            Vector2D<double>{440, 280}
        }},
        {"Assets/Maps/MapChunks/BL/b35.csv", {
            Vector2D<double>{190, 140},
            Vector2D<double>{850, 140},
            Vector2D<double>{360, 900},
            Vector2D<double>{690, 900}
        }},
        {"Assets/Maps/MapChunks/BL/c35.csv", {
            Vector2D<double>{720, 60},
            Vector2D<double>{360, 900},
            Vector2D<double>{650, 900},
            Vector2D<double>{730, 480}
        }}
    };
    chunkInfo m_BRfiles{
        {"Assets/Maps/MapChunks/BR/a35.csv", {
            Vector2D<double>{190, 150},
            Vector2D<double>{850, 150},
            Vector2D<double>{380, 880},
            Vector2D<double>{680, 890}
        }},
        {"Assets/Maps/MapChunks/BR/b35.csv", {
            Vector2D<double>{220, 880},
            Vector2D<double>{800, 860},
            Vector2D<double>{530, 70},
            Vector2D<double>{660, 290}
        }},
        {"Assets/Maps/MapChunks/BR/c35.csv", {
            Vector2D<double>{770, 890},
            Vector2D<double>{400, 870},
            Vector2D<double>{400, 60},
            Vector2D<double>{360, 450}
        }}
    };
    chunkInfo m_LRfiles{
        {"Assets/Maps/MapChunks/LR/a35.csv", {
            Vector2D<double>{250, 500},
            Vector2D<double>{500, 500},
            Vector2D<double>{600, 500},
            Vector2D<double>{800, 500}
        }},
        {"Assets/Maps/MapChunks/LR/b35.csv", {
            Vector2D<double>{190, 830},
            Vector2D<double>{890, 830},
            Vector2D<double>{190, 400},
            Vector2D<double>{890, 400}
        }},
        {"Assets/Maps/MapChunks/LR/c35.csv", {
            Vector2D<double>{130, 600},
            Vector2D<double>{890, 600},
            Vector2D<double>{430, 600},
            Vector2D<double>{600, 600}
        }}
    };

    /*std::vector<std::string> m_Tfiles{ 
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
    };*/
    /*std::vector<std::string> m_TBLfiles{};
    std::vector<std::string> m_TBRfiles{};
    std::vector<std::string> m_TLRfiles{};
    std::vector<std::string> m_BLRfiles{};
    std::vector<std::string> m_TBLRfiles{};*/

    bool readFileToChunkVector(const std::string& file);

public:
    MapChunkLoader();

    const MapChunkData& loadAndGetChunk(ChunkEntrances entrances);
    
    void printChunkVector() const;
};

