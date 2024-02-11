#include "MapGenerator.h"

#include "Attributes/RenderId.h"
#include "Entity.h"
#include "raylib.h"
#include <cstddef>

TileMap MapGenerator::createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        addStartRoom(newMap);

    return newMap;
}

void MapGenerator::updateTiles(TileMap& tileMap, const Box& area, RenderId tileId)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Vector2Int position{(area.left + x), (area.top + y)};

            tileMap.update(position, Entity{position, tileId});
        };
    }
}

void MapGenerator::addRoom(TileMap& tileMap, const Box& room)
{
    // Top wall
    updateTiles(
        tileMap,
        Box{
            room.left,
            room.top,
            room.width - 1,
            1},
        WALL_TILE);

    // Right wall
    updateTiles(
        tileMap,
        Box{
            room.right,
            room.top,
            1,
            room.height - 1},
        WALL_TILE);

    // Bottom wall
    updateTiles(
        tileMap,
        Box{
            room.left + 1,
            room.bottom,
            room.width - 1,
            1},
        WALL_TILE);

    // Left wall
    updateTiles(
        tileMap,
        Box{
            room.left,
            room.top + 1,
            1,
            room.height - 1},
        WALL_TILE);

    // Floor
    updateTiles(
        tileMap,
        Box{
            room.left + 1,
            room.top + 1,
            room.width - 2,
            room.height - 2},
        FLOOR_TILE);
}

void MapGenerator::addStartRoom(TileMap& tileMap)
{
    addRoom(
        tileMap,
        Box{
            -7,
            -7,
            15,
            15});
    updateTiles(
        tileMap,
        Box{
            -1,
            -1,
            3,
            1},
        WALL_TILE);
}