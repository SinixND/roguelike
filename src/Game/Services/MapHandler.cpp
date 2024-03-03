#include "MapHandler.h"

#include "Graphic.h"
#include "LayerID.h"
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

void MapHandler::createTiles(TileMap& tileMap, const Area& area, RenderID renderID, LayerID layerID, bool isSolid)
{
    for (int x{0}; x < area.width; ++x)
    {
        for (int y{0}; y < area.height; ++y)
        {
            Vector2i position{(area.left + x), (area.top + y)};

            Tile* tile{tileMap.createOrUpdate(position)};
            tile->position = position;
            tile->graphic.renderID = renderID;
            tile->graphic.layerID = layerID;
            tile->isSolid = isSolid;
        };
    }

    // Update global available map dimensions
    [[maybe_unused]] auto& a{dtb::mapSize()};
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
        RenderID::wall,
        LayerID::map,
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
        LayerID::map,
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
        LayerID::map,
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
        LayerID::map,
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
        LayerID::map,
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
        LayerID::map,
        true);
}