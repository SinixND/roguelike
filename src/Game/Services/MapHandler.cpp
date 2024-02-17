#include "MapHandler.h"

#include "RenderId.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

TileMap MapHandler::createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        addStartRoom(newMap);

    return newMap;
}

void MapHandler::updateTiles(TileMap& tileMap, const Area& area, RenderId renderId, bool isSolid)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Position position{(area.left + x), (area.top + y)};

            Tile tile{};
            tile.setPosition(position);
            tile.setRenderId(renderId);
            tile.setIsSolid(isSolid);

            tileMap.update(position, tile);
        };
    }
}

void MapHandler::addRoom(TileMap& tileMap, const Area& room)
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
        false);
}

void MapHandler::addStartRoom(TileMap& tileMap)
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
        true);
}