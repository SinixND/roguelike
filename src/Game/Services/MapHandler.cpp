#include "MapHandler.h"

#include "RenderID.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>

TileMap createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        MapHandler::addStartRoom(newMap);

    return newMap;
}

void MapHandler::createTiles(TileMap& tileMap, const Area& area, RenderID renderID, bool isSolid)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Vector2Int position{(area.left + x), (area.top + y)};

            auto tile{tileMap.createOrUpdate(position)};
            tile->setPosition(position);
            tile->graphic().setRenderID(renderID);
            tile->setIsSolid(isSolid);
        };
    }
}

void MapHandler::addRoom(TileMap& tileMap, const Area& room)
{
    // Top wall
    createTiles(
        tileMap,
        Area{
            room.left,
            room.top,
            room.width - 1,
            1},
        RenderID::wall,
        true);

    // Right wall
    createTiles(
        tileMap,
        Area{
            room.right,
            room.top,
            1,
            room.height - 1},
        RenderID::wall,
        true);

    // Bottom wall
    createTiles(
        tileMap,
        Area{
            room.left + 1,
            room.bottom,
            room.width - 1,
            1},
        RenderID::wall,
        true);

    // Left wall
    createTiles(
        tileMap,
        Area{
            room.left,
            room.top + 1,
            1,
            room.height - 1},
        RenderID::wall,
        true);

    // Floor
    createTiles(
        tileMap,
        Area{
            room.left + 1,
            room.top + 1,
            room.width - 2,
            room.height - 2},
        RenderID::floor,
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

    createTiles(
        tileMap,
        Area{
            -1,
            -1,
            3,
            1},
        RenderID::wall,
        true);
}