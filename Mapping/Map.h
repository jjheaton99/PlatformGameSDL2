#pragma once

#include "Tile.h"
#include "Camera.h"
#include "MapChunkLoader.h"
#include "Random.h"
#include <algorithm>
#include <array>
#include <vector>
#include <fstream>
#include <string>

//class for managing tilemapping
class Map
{
public:
    using map_type = std::vector<std::vector<Tile>>;

private:
    map_type m_map;
    MapChunkLoader m_chunkLoader{};

    std::vector<std::vector<MapChunkLoader::ChunkEntrances>> m_generatedChunks;

    std::shared_ptr<WTexture> m_tileset{ std::make_shared<WTexture>() };

    int m_levelWidth;
    int m_levelHeight;
   
    Vector2D<int> m_playerSpawnChunk;

    void generateChunks(int totalChunks);
    Tile getTileFromNumber(int number) const;
    //void switchTile(int mapRow, int mapColumn, Tile::Type type);
    void setTiles();

public:
    Map();
    ~Map();

    //random map with specified number of chunks
    void loadMap(int totalChunks);
    //overload for loading map directly from single text file
    void loadMap(const char* fileName);

    void update();
    void drawMap(const Camera& camera) const;

    const std::vector<std::vector<Tile>>& getMap() const { return m_map; }
    int getLevelWidth() const { return m_levelWidth; }
    int getLevelHeight() const { return m_levelHeight; }

    const Vector2D<int>& getPlayerSpawnChunk() const { return m_playerSpawnChunk; }

    //for testing
    void printMap() const;
};