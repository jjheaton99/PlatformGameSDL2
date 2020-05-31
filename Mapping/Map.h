#pragma once

#include "Tile.h"
#include "Camera.h"
#include "MapChunkLoader.h"
#include <array>
#include <vector>
#include <fstream>
#include <string>

//class for managing tilemapping
class Map
{
public:
    using map_type = std::vector<std::vector <Tile>>;
    using index_type = map_type::size_type;

private:
    map_type m_map;
    MapChunkLoader m_chunkLoader{};
    const int m_chunkWidth{ 25 };
    const int m_chunkHeight{ 25 };

    std::vector<std::vector<MapChunkLoader::ChunkEntrances>> m_generatedChunks;

    Tile m_background{ "Assets/MapTiles/blackGrey.png", Tile::BACKGROUND };
    Tile m_block{ "Assets/MapTiles/WhiteFadeBlocks/1.png", Tile::SOLID };
    Tile m_platform{ "Assets/MapTiles/platform.png", Tile::PLATFORM };
    Tile m_ladder{ "Assets/MapTiles/ladder.png.", Tile::LADDER };

    int m_levelWidth;
    int m_levelHeight;
   
    void generateChunks(int totalChunks);

    Tile addTile(int tileNumber);
    index_type cameraCoordToMapIndex(int coord) const;
    void setTiles();

public:
    Map();
    ~Map();

    void loadMap(int totalChunks);
    void drawMap(const Camera& camera) const;

    const std::vector<std::vector <Tile>>& getMap() const { return m_map; }
    int getLevelWidth() const { return m_levelWidth; }
    int getLevelHeight() const { return m_levelHeight; }

    //for testing
    void printMap() const;
};
