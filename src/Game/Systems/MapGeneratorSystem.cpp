#include "MapGeneratorSystem.h"
#include "Directions.h"
#include "EventId.h"
#include "Map.h"
#include "Objects.h"
#include "RNG.h"
#include "RenderId.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_set>

namespace MapGeneratorSystem
{
    void addTiles(
        Tiles& tiles,
        RectangleExI const& rectangle,
        RenderId renderId,
        bool isSolid = false,
        bool isOpaque = false)
    {
        for (int x{0}; x < rectangle.width(); ++x)
        {
            for (int y{0}; y < rectangle.height(); ++y)
            {
                tiles.create(
                    Vector2I{
                        rectangle.left() + x,
                        rectangle.top() + y},
                    renderId,
                    isSolid,
                    isOpaque);
            }
        }
    }

    //* Add room (floor with surrounding walls)
    void addRoom(Tiles& tiles, RectangleExI const& room)
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
            RenderId::WALL,
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
            RenderId::WALL,
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
            RenderId::WALL,
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
            RenderId::WALL,
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
            RenderId::FLOOR,
            false,
            false);
    }

    Map createGridRooms(int level)
    {
        Map map{};

        Vector2I roomPosition{0, 0};
        int const roomWidth{15};
        int maxRoomOffset{(2 + level) * roomWidth};

        //* Add first room
        addRoom(
            map.tiles,
            RectangleExI{
                roomPosition,
                roomWidth,
                roomWidth});

        std::unordered_set<Vector2I> usedPositions{roomPosition};

        //* Take random direction and add room
        while (Vector2Sum(roomPosition) < maxRoomOffset)
        {

            //* (Update) Old room position used for room connection
            Vector2I oldRoomPosition{roomPosition};

            //* Choose random direction
            Vector2I direction{Directions::directions[snx::RNG::random(0, 3)]};

            //* Update new room position
            roomPosition += Vector2Scale(direction, roomWidth);

            //* Add new room if room position unused
            if (!usedPositions.contains(roomPosition))
            {
                usedPositions.insert(roomPosition);

                addRoom(
                    map.tiles,
                    RectangleExI{
                        roomPosition,
                        roomWidth,
                        roomWidth});
            }

            //* Add connection gap in wall between old and new room
            addTiles(
                map.tiles,
                RectangleExI{
                    oldRoomPosition,
                    roomPosition},
                RenderId::FLOOR);
        }

        //* Add previous level trigger
        createObject(
            &map.objects,
            Vector2I{
                0,
                0},
            RenderId::ASCEND,
            "Stairs",
            "Ascend",
            EventId::PREVIOUS_LEVEL);

        // Add next level trigger
        // setObject(
        //     map.objects_,
        //     "Descend",
        //     Vector2I{
        //         0,
        //         -5},
        //     RenderId::descend,
        //     EventId::nextLevel);

        //* Add enemies
        initEnemies(
            &map.enemies,
            level,
            map);

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
            RenderId::WALL,
            true,
            true);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                4,
                -5,
                1,
                5},
            RenderId::FLOOR);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -4,
                3,
                1},
            RenderId::FLOOR);

        addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -2,
                3,
                1},
            RenderId::FLOOR);

        //* Tiles
        testRoom.tiles.create(
            Vector2I{
                0,
                -1},
            RenderId::WALL,
            true,
            true);

        testRoom.tiles.create(
            Vector2I{
                5,
                6},
            RenderId::WALL,
            true,
            true);

        testRoom.tiles.create(
            Vector2I{
                6,
                5},
            RenderId::WALL,
            true,
            true);

        testRoom.tiles.create(
            Vector2I{
                5,
                5},
            RenderId::WALL,
            true,
            true);

        testRoom.tiles.create(
            Vector2I{
                -6,
                5},
            RenderId::FLOOR);

        //* Next level trigger
        createObject(
            &testRoom.objects,
            Vector2I{
                0,
                -5},
            RenderId::DESCEND,
            "Stairs",
            "Descend",
            EventId::NEXT_LEVEL);

        createEnemy(
            &testRoom.enemies,
            testRoom,
            RenderId::GOBLIN,
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
        createObject(
            &startRoom.objects,
            Vector2I{
                0,
                -5},
            RenderId::DESCEND,
            "Stairs",
            "Descend",
            EventId::NEXT_LEVEL);

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
