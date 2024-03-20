#include "MapHandler.h"

#include "Constants.h"
#include "Graphic.h"
#include "LayerID.h"
#include "RNG.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "Tile.h"
#include "TileMap.h"
#include "Utility.h"
#include "raylibEx.h"
#include <array>
#include <cstddef>
#include <raylib.h>
#include <vector>

namespace MapHandler
{
    TileMap createNewMap(size_t level)
    {
        TileMap newMap{};

        if (!level)
        {
            createStartRoom(newMap);
            return newMap;
        }

        // Choose map design
        switch (1) // RNG::random(1, 2)
        {
            case 1:
                createGridRooms(newMap, level);
                break;

            default:
                break;
        }

        return newMap;
    }

    void addTiles(
        TileMap& tileMap,
        Area const& area,
        Graphic graphic,
        bool isSolid,
        bool blocksVision)
    {
        for (size_t x{0}; x < area.width; ++x)
        {
            for (size_t y{0}; y < area.height; ++y)
            {
                Vector2i position{static_cast<int>((area.left + x)), static_cast<int>((area.top + y))};

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

    void addRoom(TileMap& tileMap, Area const& room)
    {
        // Return if area width or height < 2
        if (room.width < 2 || room.height < 2)
            return;

        // Top wall
        addTiles(
            tileMap,
            Area{
                room.left,
                room.top,
                static_cast<size_t>(room.width - 1),
                static_cast<size_t>(1)},
            Graphic(
                RenderID::WALL,
                LayerID::MAP),
            true,
            true);

        // Right wall
        addTiles(
            tileMap,
            Area{
                room.right,
                room.top,
                static_cast<size_t>(1),
                static_cast<size_t>(room.height - 1)},
            Graphic(
                RenderID::WALL,
                LayerID::MAP),
            true,
            true);

        // Bottom wall
        addTiles(
            tileMap,
            Area{
                room.left + 1,
                room.bottom,
                static_cast<size_t>(room.width - 1),
                static_cast<size_t>(1)},
            Graphic(
                RenderID::WALL,
                LayerID::MAP),
            true,
            true);

        // Left wall
        addTiles(
            tileMap,
            Area{
                room.left,
                room.top + 1,
                static_cast<size_t>(1),
                static_cast<size_t>(room.height - 1)},
            Graphic(
                RenderID::WALL,
                LayerID::MAP),
            true,
            true);

        // Floor
        addTiles(
            tileMap,
            Area{
                room.left + 1,
                room.top + 1,
                static_cast<size_t>(room.width - 2),
                static_cast<size_t>(room.height - 2)},
            Graphic(
                RenderID::FLOOR,
                LayerID::MAP),
            false,
            false);
    }

    void createStartRoom(TileMap& tileMap)
    {
        addRoom(
            tileMap,
            Area{
                -7,
                -7,
                15,
                15});

        addTiles(
            tileMap,
            Area{
                -1,
                -1,
                3,
                1},
            Graphic(
                RenderID::WALL,
                LayerID::MAP),
            true,
            true);
    }

    void createGridRooms(TileMap& tileMap, size_t level)
    {
        static const std::array<Vector2i, 4> directions{
            V_LEFT,
            V_RIGHT,
            V_UP,
            V_DOWN};

        Vector2i roomPosition{0, 0};
        const size_t roomDimension{15};
        size_t maxRoomOffset{(1 + level) * roomDimension};

        std::vector<Vector2i> usedPositions{};

        // Add first room
        addRoom(
            tileMap,
            Area{
                roomPosition,
                roomDimension,
                roomDimension});

        // Take random direction and add room
        while (true)
        {
            Vector2i oldRoomPosition{roomPosition};
            usedPositions.push_back(oldRoomPosition);

            //* do
            //* {
            // Choose random direction
            Vector2i direction{directions[RNG::random(0, 3)]};

            // New room position
            roomPosition = Vector2Add(
                oldRoomPosition,
                Vector2Scale(
                    direction,
                    static_cast<int>(roomDimension)));

            // Break if out of range
            if (Vector2Sum(roomPosition) > (2 * maxRoomOffset))
                break;

            if (!Utility::isInVector(roomPosition, usedPositions))
                addRoom(
                    tileMap,
                    Area{
                        roomPosition,
                        roomDimension,
                        roomDimension});

            // Add gap in wall
            addTiles(
                tileMap,
                Area{
                    oldRoomPosition,
                    roomPosition},
                Graphic{
                    RenderID::FLOOR,
                    LayerID::MAP},
                false,
                false);
        }
    }
}