#include "MapGenerator.h"

#include "Directions.h"
#include "Graphic.h"
#include "LayerID.h"
#include "Position.h"
#include "RNG.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "Tile.h"
#include "TileMap.h"
#include "Utility.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <array>
#include <raylib.h>
#include <string>
#include <vector>

namespace MapGenerator
{
    auto createRandomMap(int level) -> TileMap
    {
        TileMap newMap{};

        // Choose map design
        switch (1) //* RNG::random(1, 2)
        {
        case 1:
            createGridRooms(newMap, level);
            break;

        default:
            break;
        }

        return newMap;
    }

    void addTile(
        TileMap& tileMap,
        std::string tag,
        Vector2I const& position,
        GraphicComponent graphic,
        VisibilityID visibility,
        bool isSolid,
        bool blocksVision)
    {
        tileMap.createOrUpdate(
            position,
            Tile(
                tag,
                PositionComponent(position),
                graphic,
                visibility,
                isSolid,
                blocksVision));

        // Update global available map dimensions
        dtb::extendMapsize(position);
    }

    void addTiles(
        TileMap& tileMap,
        std::string tag,
        RectangleExI const& rectangle,
        GraphicComponent graphic,
        VisibilityID visibility,
        bool isSolid,
        bool blocksVision)
    {
        for (int x{0}; x < rectangle.width; ++x)
        {
            for (int y{0}; y < rectangle.height; ++y)
            {
                addTile(
                    tileMap,
                    tag,
                    Vector2I{
                        rectangle.left + x,
                        rectangle.top + y},
                    graphic,
                    visibility,
                    isSolid,
                    blocksVision);
            }
        }
    }

    // Add room (floor with surrounding walls)
    void addRoom(TileMap& tileMap, RectangleExI const& room)
    {
        if (room.width < 2 || room.height < 2)
        {
            return;
        }

        // Top wall
        addTiles(
            tileMap,
            "Wall",
            RectangleExI{
                room.left,
                room.top,
                room.width - 1,
                1},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        // Right wall
        addTiles(
            tileMap,
            "Wall",
            RectangleExI{
                room.right,
                room.top,
                1,
                room.height - 1},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        // Bottom wall
        addTiles(
            tileMap,
            "Wall",
            RectangleExI{
                room.left + 1,
                room.bottom,
                room.width - 1,
                1},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        // Left wall
        addTiles(
            tileMap,
            "Wall",
            RectangleExI{
                room.left,
                room.top + 1,
                1,
                room.height - 1},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        // Floor
        addTiles(
            tileMap,
            "Floor",
            RectangleExI{
                room.left + 1,
                room.top + 1,
                room.width - 2,
                room.height - 2},
            GraphicComponent(RenderID::FLOOR, LayerID::MAP),
            VisibilityID::UNSEEN,
            false,
            false);
    }

    auto createStartRoom() -> TileMap
    {
        TileMap startRoom{};

        addRoom(
            startRoom,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                15,
                15});

        // Add walls
        addTiles(
            startRoom,
            "Wall",
            RectangleExI{-1, 2, 3, 1},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        addTiles(
            startRoom,
            "Wall",
            RectangleExI{-2, 0, 1, 2},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        addTiles(
            startRoom,
            "Wall",
            RectangleExI{2, 0, 1, 2},
            GraphicComponent(RenderID::WALL, LayerID::MAP),
            VisibilityID::UNSEEN,
            true,
            true);

        // Add next level trigger
        addTile(
            startRoom,
            "Stairs",
            Vector2I{0, -5},
            GraphicComponent(RenderID::NEXT_LEVEL, LayerID::MAP),
            VisibilityID::UNSEEN,
            false,
            false);

        return startRoom;
    }

    void createGridRooms(TileMap& tileMap, int level)
    {
        static std::array<Vector2I, 4> const directions{
            Directions::V_LEFT,
            Directions::V_RIGHT,
            Directions::V_UP,
            Directions::V_DOWN};

        Vector2I roomPosition{0, 0};
        int const roomWidth{15};
        int maxRoomOffset{(2 + level) * roomWidth};

        // Add first room
        addRoom(
            tileMap,
            RectangleExI{
                roomPosition,
                roomWidth,
                roomWidth});

        std::vector<Vector2I> usedPositions{roomPosition};

        // Take random direction and add room
        while (Vector2Sum(roomPosition) < maxRoomOffset)
        {

            // (Update) Old room position used for room connection
            Vector2I oldRoomPosition{roomPosition};

            // Choose random direction
            Vector2I direction{directions[snx::RNG::random(0, 3)]};

            // Update new room position
            roomPosition += Vector2Scale(
                direction,
                roomWidth);

            // Add new room if room position unused
            if (!snx::isInVector(roomPosition, usedPositions))
            {
                usedPositions.push_back(roomPosition);

                addRoom(
                    tileMap,
                    RectangleExI{roomPosition, roomWidth, roomWidth});
            }

            // Add connection gap in wall between old and new room
            addTiles(
                tileMap,
                "Floor",
                RectangleExI{oldRoomPosition, roomPosition},
                GraphicComponent{RenderID::FLOOR, LayerID::MAP},
                VisibilityID::UNSEEN,
                false,
                false);
        }
    }
}