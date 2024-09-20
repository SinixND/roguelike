#include "MapGenerator.h"

#include "Directions.h"
#include "Event.h"
#include "Map.h"
#include "Objects.h"
#include "RNG.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_set>

namespace MapGenerator
{
    // Access or insert
    void addTile(
        Tiles& tiles,
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false)
    {
        tiles.set(tilePosition, renderID, isSolid, isOpaque);
    }

    void addObject(
        Objects& objects,
        std::string tag,
        Vector2I const& tilePosition,
        RenderID renderID,
        Event event = Event::null)
    {
        objects.set(tilePosition, renderID, tag, event);
    }

    void addTiles(
        Tiles& tiles,
        RectangleExI const& rectangle,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false)
    {
        for (int x{0}; x < rectangle.width(); ++x)
        {
            for (int y{0}; y < rectangle.height(); ++y)
            {
                addTile(
                    tiles,
                    Vector2I{
                        rectangle.left() + x,
                        rectangle.top() + y},
                    renderID,
                    isSolid,
                    isOpaque);
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
            RectangleExI{
                room.left(),
                room.top(),
                room.width() - 1,
                1},
            RenderID::wall,
            true,
            true);

        // Right wall
        addTiles(
            tiles,
            RectangleExI{
                room.right(),
                room.top(),
                1,
                room.height() - 1},
            RenderID::wall,
            true,
            true);

        // Bottom wall
        addTiles(
            tiles,
            RectangleExI{
                room.left() + 1,
                room.bottom(),
                room.width() - 1,
                1},
            RenderID::wall,
            true,
            true);

        // Left wall
        addTiles(
            tiles,
            RectangleExI{
                room.left(),
                room.top() + 1,
                1,
                room.height() - 1},
            RenderID::wall,
            true,
            true);

        // Floor
        addTiles(
            tiles,
            RectangleExI{
                room.left() + 1,
                room.top() + 1,
                room.width() - 2,
                room.height() - 2},
            RenderID::floor,
            false,
            false);
    }

    Map createGridRooms(int level)
    {
        Map map{};

        Vector2I roomPosition{0, 0};
        int const roomWidth{15};
        int maxRoomOffset{(2 + level) * roomWidth};

        // Add first room
        addRoom(
            map.tiles_,
            RectangleExI{
                roomPosition,
                roomWidth,
                roomWidth});

        std::unordered_set<Vector2I> usedPositions{roomPosition};

        // Take random direction and add room
        while (Vector2Sum(roomPosition) < maxRoomOffset)
        {

            // (Update) Old room position used for room connection
            Vector2I oldRoomPosition{roomPosition};

            // Choose random direction
            Vector2I direction{Directions::directions[snx::RNG::random(0, 3)]};

            // Update new room position
            roomPosition += Vector2Scale(direction, roomWidth);

            // Add new room if room position unused
            if (!usedPositions.contains(roomPosition))
            {
                usedPositions.insert(roomPosition);

                addRoom(
                    map.tiles_,
                    RectangleExI{
                        roomPosition,
                        roomWidth,
                        roomWidth});
            }

            // Add connection gap in wall between old and new room
            addTiles(
                map.tiles_,
                RectangleExI{
                    oldRoomPosition,
                    roomPosition},
                RenderID::floor);
        }

        // Add previous level trigger
        addObject(
            map.objects_,
            "Ascend",
            Vector2I{
                0,
                0},
            RenderID::ascend,
            Event::previousLevel);

        // Add next level trigger
        // addObject(
        //     map.objects_,
        //     "Descend",
        //     Vector2I{
        //         0,
        //         -5},
        //     RenderID::descend,
        //     Event::nextLevel);

        // Add enemies
        map.enemies_.init(level, map.tiles_);

        return map;
    }

    Map createTestRoom()
    {
        Map testRoom{};

        // Rooms
        addRoom(
            testRoom.tiles_,
            RectangleExI{
                Vector2I{
                    -75,
                    -75},
                151,
                151});

        addRoom(
            testRoom.tiles_,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                15,
                15});

        addRoom(
            testRoom.tiles_,
            RectangleExI{
                -7,
                2,
                7,
                4});

        addRoom(
            testRoom.tiles_,
            RectangleExI{
                3,
                -5,
                3,
                5});

        // Add walls
        addTiles(
            testRoom.tiles_,
            RectangleExI{
                1,
                0,
                1,
                8},
            RenderID::wall,
            true,
            true);

        addTiles(
            testRoom.tiles_,
            RectangleExI{
                4,
                -5,
                1,
                5},
            RenderID::floor);

        addTiles(
            testRoom.tiles_,
            RectangleExI{
                3,
                -4,
                3,
                1},
            RenderID::floor);

        addTiles(
            testRoom.tiles_,
            RectangleExI{
                3,
                -2,
                3,
                1},
            RenderID::floor);

        // Tiles
        addTile(
            testRoom.tiles_,
            Vector2I{
                0,
                -1},
            RenderID::wall,
            true,
            true);

        addTile(
            testRoom.tiles_,
            Vector2I{
                5,
                6},
            RenderID::wall,
            true,
            true);

        addTile(
            testRoom.tiles_,
            Vector2I{
                6,
                5},
            RenderID::wall,
            true,
            true);

        addTile(
            testRoom.tiles_,
            Vector2I{
                5,
                5},
            RenderID::wall,
            true,
            true);

        addTile(
            testRoom.tiles_,
            Vector2I{
                -6,
                5},
            RenderID::floor);

        // Next level trigger
        addObject(
            testRoom.objects_,
            "Descend",
            Vector2I{
                0,
                -5},
            RenderID::descend,
            Event::nextLevel);

        testRoom.enemies_.create(
            testRoom.tiles_,
            RenderID::goblin,
            Vector2I{3, 0});

        return testRoom;
    }

    Map createStartRoom()
    {
        Map startRoom{};

        addRoom(
            startRoom.tiles_,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                15,
                15});

        // Add next level trigger
        addObject(
            startRoom.objects_,
            "Descend",
            Vector2I{
                0,
                -5},
            RenderID::descend,
            Event::nextLevel);

        return startRoom;
    }

    Map createRandomMap(int level)
    {
        Map newMap{};

        // Choose map design
        switch (1) //* RNG::random(1, 2)
        {
        case 1:
            newMap = createGridRooms(level);
            break;

        default:
            break;
        }

        return newMap;
    }
}
