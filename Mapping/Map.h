#pragma once

#include "Tile.h"
#include "Camera.h"
#include "MapChunkLoader.h"
#include "Random.h"
#include <array>
#include <vector>
#include <fstream>
#include <string>

//class for managing tilemapping
class Map
{
public:
    using map_type = std::vector<std::vector<std::unique_ptr<Tile>>>;
    using index_type = map_type::size_type;

private:
    map_type m_map;
    MapChunkLoader m_chunkLoader{};

    std::vector<std::vector<MapChunkLoader::ChunkEntrances>> m_generatedChunks;

    int m_levelWidth;
    int m_levelHeight;
   
    void generateChunks(int totalChunks);

    Tile::Type getTileTypeFromNumber(int tileNumber) const;
    std::string getTileFileFromNumber(int tileNumber) const;
    index_type cameraCoordToMapIndex(int coord) const;
    void setTiles();

public:
    Map();
    ~Map();

    void loadMap(int totalChunks);
    void drawMap(const Camera& camera) const;

    const std::vector<std::vector<std::unique_ptr<Tile>>>& getMap() const { return m_map; }
    int getLevelWidth() const { return m_levelWidth; }
    int getLevelHeight() const { return m_levelHeight; }

    //for testing
    void printMap() const;
};
