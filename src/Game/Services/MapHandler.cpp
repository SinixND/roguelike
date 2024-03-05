#include "MapHandler.h"

#include "Graphic.h"
#include "LayerID.h"
#include "RenderID.h"
#include "Tile.h"
#include "TileMap.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <memory>

TileMap createNewMap(size_t level)
{
    TileMap newMap{};

    if (level == 0)
        MapHandler::addStartRoom(newMap);

    return newMap;
}

void MapHandler::createTiles(
    TileMap& tileMap,
    const Area& area,
    Graphic graphic,
    bool isSolid,
    bool blocksVision)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Vector2i position{(area.left + x), (area.top + y)};

            tileMap.createOrUpdate(
                position,
                Tile(
                    position,
                    graphic,
                    isSolid,
                    blocksVision));
        };
    }

    // Update global available map dimensions
    dtb::updateMapSize({area.left, area.top});
    dtb::updateMapSize({area.right, area.bottom});
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
        Graphic(
            RenderID::wall,
            LayerID::map),
        true,
        true);

    // Right wall
    createTiles(
        tileMap,
        Area{
            room.right,
            room.top,
            1,
            room.height - 1},
        Graphic(
            RenderID::wall,
            LayerID::map),
        true,
        true);

    // Bottom wall
    createTiles(
        tileMap,
        Area{
            room.left + 1,
            room.bottom,
            room.width - 1,
            1},
        Graphic(
            RenderID::wall,
            LayerID::map),
        true,
        true);

    // Left wall
    createTiles(
        tileMap,
        Area{
            room.left,
            room.top + 1,
            1,
            room.height - 1},
        Graphic(
            RenderID::wall,
            LayerID::map),
        true,
        true);

    // Floor
    createTiles(
        tileMap,
        Area{
            room.left + 1,
            room.top + 1,
            room.width - 2,
            room.height - 2},
        Graphic(
            RenderID::floor,
            LayerID::map),
        false,
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
        Graphic(
            RenderID::wall,
            LayerID::map),
        true,
        true);
}