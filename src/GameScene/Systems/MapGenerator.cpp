#include "MapGenerator.h"

#include "Directions.h"
#include "RNG.h"
#include "RenderID.h"
#include "Tiles.h"
#include "Utility.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <array>
#include <raylib.h>
#include <string>
#include <vector>

namespace MapGenerator
{
    void addTile(
        Tiles& tiles,
        std::string tag,
        Vector2I const& tilePosition,
        RenderID renderID,
        VisibilityID visibilityID,
        bool isSolid,
        bool blocksVision)
    {
        tiles.insert(tilePosition, renderID, tag, visibilityID, isSolid, blocksVision);
    }

    void addTiles(
        Tiles& tiles,
        std::string tag,
        RectangleExI const& rectangle,
        RenderID renderID,
        VisibilityID visibilityID,
        bool isSolid,
        bool blocksVision)
    {
        for (int x{0}; x < rectangle.width(); ++x)
        {
            for (int y{0}; y < rectangle.height(); ++y)
            {
                addTile(
                    tiles,
                    tag,
                    Vector2I{
                        rectangle.left() + x,
                        rectangle.top() + y},
                    renderID,
                    visibilityID,
                    isSolid,
                    blocksVision);
            }
        }
    }

    // Add room (floor with surrounding walls)
    void addRoom(Tiles& tiles, RectangleExI const& room)
    {
        if (room.width() < 2 || room.height() < 2)
        {
            return;
        }

        // Top wall
        addTiles(
            tiles,
            "Wall",
            RectangleExI{
                room.left(),
                room.top(),
                room.width() - 1,
                1},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        // Right wall
        addTiles(
            tiles,
            "Wall",
            RectangleExI{
                room.right(),
                room.top(),
                1,
                room.height() - 1},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        // Bottom wall
        addTiles(
            tiles,
            "Wall",
            RectangleExI{
                room.left() + 1,
                room.bottom(),
                room.width() - 1,
                1},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        // Left wall
        addTiles(
            tiles,
            "Wall",
            RectangleExI{
                room.left(),
                room.top() + 1,
                1,
                room.height() - 1},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        // Floor
        addTiles(
            tiles,
            "Floor",
            RectangleExI{
                room.left() + 1,
                room.top() + 1,
                room.width() - 2,
                room.height() - 2},
            RenderID::floor,
            VisibilityID::invisible,
            false,
            false);
    }

    void createGridRooms(Tiles& tiles, int level)
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
            tiles,
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
            roomPosition += Vector2Scale(direction, roomWidth);

            // Add new room if room position unused
            if (!snx::isInVector(roomPosition, usedPositions))
            {
                usedPositions.push_back(roomPosition);

                addRoom(
                    tiles,
                    RectangleExI{
                        roomPosition,
                        roomWidth,
                        roomWidth});
            }

            // Add connection gap in wall between old and new room
            addTiles(
                tiles,
                "Floor",
                RectangleExI{
                    oldRoomPosition,
                    roomPosition},
                RenderID::floor,
                VisibilityID::invisible,
                false,
                false);
        }
    }

    Tiles createStartRoom()
    {
        Tiles startRoom{};

        addRoom(
            startRoom,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                151,
                151});

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
            RectangleExI{
                -1,
                2,
                3,
                1},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        addTiles(
            startRoom,
            "Wall",
            RectangleExI{
                -2,
                0,
                1,
                2},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        addTiles(
            startRoom,
            "Wall",
            RectangleExI{
                2,
                0,
                1,
                2},
            RenderID::wall,
            VisibilityID::invisible,
            true,
            true);

        // Add next level trigger
        addTile(
            startRoom,
            "Stairs",
            Vector2I{
                0,
                -5},
            RenderID::nextLevel,
            VisibilityID::invisible,
            false,
            false);

        return startRoom;
    }

    Tiles createRandomMap(int level)
    {
        Tiles newMap{};

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
}
