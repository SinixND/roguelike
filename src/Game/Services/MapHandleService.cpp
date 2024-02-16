#include "MapHandleService.h"

#include "LayerId.h"
#include "RenderData.h"
#include "RenderId.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

TileMap MapHandleService::createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        addStartRoom(newMap);

    return newMap;
}

void MapHandleService::updateTiles(TileMap& tileMap, const Area& area, RenderId renderId, LayerId layerId, bool isSolid)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Position position{(area.left + x), (area.top + y)};

            Tile tile{};
            tile.setPosition(position);
            tile.setRenderData({renderId, layerId});
            tile.setIsSolid(isSolid);

            tileMap.update(position, tile);
        };
    }
}

void MapHandleService::addRoom(TileMap& tileMap, const Area& room)
{
    // Top wall
    updateTiles(
        tileMap,
        Area{
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
        Area{
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
        Area{
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
        Area{
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
        Area{
            room.left + 1,
            room.top + 1,
            room.width - 2,
            room.height - 2},
        RENDER_FLOOR_TILE,
        LAYER_MAP,
        false);
}

void MapHandleService::addStartRoom(TileMap& tileMap)
{
    addRoom(
        tileMap,
        Area{
            -7,
            -7,
            15,
            15});

    updateTiles(
        tileMap,
        Area{
            -1,
            -1,
            3,
            1},
        RENDER_WALL_TILE,
        LAYER_MAP,
        true);
}