#include "TileGenerator.h"

#include "Entity.h"
#include "LayerId.h"
#include "RenderId.h"
#include "raylib.h"
#include <cstddef>

TileMap TileGenerator::createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        addStartRoom(newMap);

    return newMap;
}

void TileGenerator::updateTiles(TileMap& tileMap, const Box& area, RenderId renderId, LayerId layerId, bool isSolid)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Vector2Int position{(area.left + x), (area.top + y)};

            tileMap.update(
                position,
                Tile{
                    {position,
                     {renderId, layerId}},
                    isSolid});
        };
    }
}

void TileGenerator::addRoom(TileMap& tileMap, const Box& room)
{
    // Top wall
    updateTiles(
        tileMap,
        Box{
            room.left,
            room.top,
            room.width - 1,
            1},
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);

    // Right wall
    updateTiles(
        tileMap,
        Box{
            room.right,
            room.top,
            1,
            room.height - 1},
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);

    // Bottom wall
    updateTiles(
        tileMap,
        Box{
            room.left + 1,
            room.bottom,
            room.width - 1,
            1},
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);

    // Left wall
    updateTiles(
        tileMap,
        Box{
            room.left,
            room.top + 1,
            1,
            room.height - 1},
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);

    // Floor
    updateTiles(
        tileMap,
        Box{
            room.left + 1,
            room.top + 1,
            room.width - 2,
            room.height - 2},
        RENDER_FLOOR_TILE,
        LAYER_MAP,
        false);
}

void TileGenerator::addStartRoom(TileMap& tileMap)
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
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);
}