#include "MapGeneratorSystem.h"
#include "Directions.h"
#include "Event.h"
#include "Map.h"
#include "ObjectSoA.h"
#include "RNG.h"
#include "RenderID.h"
#include "TileSoA.h"
#include "raylibEx.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_set>

namespace MapGeneratorSystem
{
    //* Access or insert
    void addTile(
        TileSoA& tiles,
        Vector2I const& tilePosition,
        RenderID renderID,
        bool isSolid = false,
        bool isOpaque = false)
    {
        tiles.set(tilePosition, renderID, isSolid, isOpaque);
    }

    void addObject(
        ObjectSoA& objects,
        std::string name,
        std::string action,
        Vector2I const& tilePosition,
        RenderID renderID,
        Event event = Event::NONE)
    {
        objects.set(tilePosition, renderID, name, action, event);
    }

    void addTiles(
        TileSoA& tiles,
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

    //* Add room (floor with surrounding walls)
    void addRoom(TileSoA& tiles, RectangleExI const& room)
    {
        if (room.width() < 2 || room.height() < 2)
        {
            return;
        }

        //* Top wall
        addTiles(
            tiles,
            RectangleExI{
                room.left(),
                room.top(),
                room.width() - 1,
                1},
            RenderID::WALL,
            true,
            true);

        //* Right wall
        addTiles(
            tiles,
            RectangleExI{
                room.right(),
                room.top(),
                1,
                room.height() - 1},
            RenderID::WALL,
            true,
            true);

        //* Bottom wall
        addTiles(
            tiles,
            RectangleExI{
                room.left() + 1,
                room.bottom(),
                room.width() - 1,
                1},
            RenderID::WALL,
            true,
            true);

        //* Left wall
        addTiles(
            tiles,
            RectangleExI{
                room.left(),
                room.top() + 1,
                1,
                room.height() - 1},
            RenderID::WALL,
            true,
            true);

        //* Floor
        addTiles(
            tiles,
            RectangleExI{
                room.left() + 1,
                room.top() + 1,
                room.width() - 2,
                room.height() - 2},
            RenderID::FLOOR,
            false,
            false);
    }

    Map createGridRooms(int level)
    {
        Map map{};

        Vector2I roomPosition{0, 0};
        int const ROOM_WIDTH{15};
        int maxRoomOffset{(2 + level) * ROOM_WIDTH};

        //* Add first room
        addRoom(
            map.tiles,
            RectangleExI{
                roomPosition,
                ROOM_WIDTH,
                ROOM_WIDTH});

        std::unordered_set<Vector2I> usedPositions{roomPosition};

        //* Take random direction and add room
        while (Vector2Sum(roomPosition) < maxRoomOffset)
        {

            //* (Update) Old room position used for room connection
            Vector2I oldRoomPosition{roomPosition};

            //* Choose random direction
            Vector2I direction{Directions::DIRECTIONS[snx::RNG::random(0, 3)]};

            //* Update new room position
            roomPosition += Vector2Scale(direction, ROOM_WIDTH);

            //* Add new room if room position unused
            if (!usedPositions.contains(roomPosition))
            {
                usedPositions.insert(roomPosition);

                addRoom(
                    map.tiles,
                    RectangleExI{
                        roomPosition,
                        ROOM_WIDTH,
                        ROOM_WIDTH});
            }

            //* Add connection gap in wall between old and new room
            addTiles(
                map.tiles,
                RectangleExI{
                    oldRoomPosition,
                    roomPosition},
                RenderID::FLOOR);
        }

        //* Add previous level trigger
        addObject(
            map.objects,
            "Stairs",
            "Ascend",
            Vector2I{
                0,
                0},
            RenderID::ASCEND,
            Event::PREVIOUS_LEVEL);

        // Add next level trigger
        // addObject(
        //     map.objects_,
        //     "Descend",
        //     Vector2I{
        //         0,
        //         -5},
        //     RenderID::descend,
        //     Event::nextLevel);

        //* Add enemies
        map.enemies.init(level, map);

        return map;
    }

    Map createTestRoom()
    {
        Map testRoom{};

        //* Rooms
        addRoom(
            testRoom.tiles,
            RectangleExI{
                Vector2I{
                    -75,
                    -75},
                151,
                151});

        addRoom(
            testRoom.tiles,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                15,
                15});

        addRoom(
            testRoom.tiles,
            RectangleExI{
                -7,
                2,
                7,
                4});

        addRoom(
            testRoom.tiles,
            RectangleExI{
                3,
                -5,
                3,
                5});

        //* Add walls
        addTiles(
            testRoom.tiles,
            RectangleExI{
                1,
                0,
                1,
                8},
            RenderID::WALL,
            true,
            true);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                4,
                -5,
                1,
                5},
            RenderID::FLOOR);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -4,
                3,
                1},
            RenderID::FLOOR);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -2,
                3,
                1},
            RenderID::FLOOR);

        //* TileSoA
        addTile(
            testRoom.tiles,
            Vector2I{
                0,
                -1},
            RenderID::WALL,
            true,
            true);

        addTile(
            testRoom.tiles,
            Vector2I{
                5,
                6},
            RenderID::WALL,
            true,
            true);

        addTile(
            testRoom.tiles,
            Vector2I{
                6,
                5},
            RenderID::WALL,
            true,
            true);

        addTile(
            testRoom.tiles,
            Vector2I{
                5,
                5},
            RenderID::WALL,
            true,
            true);

        addTile(
            testRoom.tiles,
            Vector2I{
                -6,
                5},
            RenderID::FLOOR);

        //* Next level trigger
        addObject(
            testRoom.objects,
            "Stairs",
            "Descend",
            Vector2I{
                0,
                -5},
            RenderID::DESCEND,
            Event::NEXT_LEVEL);

        testRoom.enemies.create(
            testRoom,
            RenderID::GOBLIN,
            false,
            Vector2I{3, 0});

        return testRoom;
    }

    Map createStartRoom()
    {
        Map startRoom{};

        addRoom(
            startRoom.tiles,
            RectangleExI{
                Vector2I{
                    0,
                    0},
                15,
                15});

        //* Add next level trigger
        addObject(
            startRoom.objects,
            "Stairs",
            "Descend",
            Vector2I{
                0,
                -5},
            RenderID::DESCEND,
            Event::NEXT_LEVEL);

        return startRoom;
    }

    Map createRandomMap(int level)
    {
        Map newMap{};

        //* Choose map design
        switch (1) //* RNG::random(1, 2)
        {
            default:
            case 1:
            {
                newMap = createGridRooms(level);

                break;
            }
        }

        return newMap;
    }
}
