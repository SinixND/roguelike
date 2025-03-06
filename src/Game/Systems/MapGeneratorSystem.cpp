#include "MapGeneratorSystem.h"

#include "Directions.h"
#include "Enemies.h"
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

[[nodiscard]]
Tiles const& addTiles(
    Tiles& tiles,
    RectangleExI const& rectangle,
    RenderId renderId,
    bool isSolid = false,
    bool isOpaque = false
)
{
    for ( int x{ 0 }; x < rectangle.width(); ++x )
    {
        for ( int y{ 0 }; y < rectangle.height(); ++y )
        {
            tiles.insert(
                Vector2I{
                    rectangle.left() + x,
                    rectangle.top() + y
                },
                renderId,
                isSolid,
                isOpaque
            );
        }
    }

    return tiles;
}

//* Add room (floor with surrounding walls)
[[nodiscard]]
Tiles const& addRoom(
    Tiles& tilesIO,
    RectangleExI const& room
)
{
    if ( room.width() < 2 || room.height() < 2 )
    {
        tilesIO.insert(
            Vector2I{ room.left(), room.top() },
            RenderId::WALL,
            true,
            true
        );

        return tilesIO;
    }

    //* Floor
    tilesIO = addTiles(
        tilesIO,
        RectangleExI{
            room.left(),
            room.top(),
            room.width(),
            room.height()
        },
        RenderId::FLOOR,
        false,
        false
    );

    //* Top wall
    tilesIO = addTiles(
        tilesIO,
        RectangleExI{
            room.left(),
            room.top(),
            room.width() - 1,
            1
        },
        RenderId::WALL,
        true,
        true
    );

    //* Right wall
    tilesIO = addTiles(
        tilesIO,
        RectangleExI{
            room.right(),
            room.top(),
            1,
            room.height() - 1
        },
        RenderId::WALL,
        true,
        true
    );

    //* Bottom wall
    tilesIO = addTiles(
        tilesIO,
        RectangleExI{
            room.left() + 1,
            room.bottom(),
            room.width() - 1,
            1
        },
        RenderId::WALL,
        true,
        true
    );

    //* Left wall
    tilesIO = addTiles(
        tilesIO,
        RectangleExI{
            room.left(),
            room.top() + 1,
            1,
            room.height() - 1
        },
        RenderId::WALL,
        true,
        true
    );

    return tilesIO;
}

namespace MapGeneratorSystem
{
    Map createTestRoom()
    {
        Map testRoom{};

        //* Rooms
        testRoom.tiles = addRoom(
            testRoom.tiles,
            RectangleExI{
                Vector2I{
                    -75,
                    -75
                },
                151,
                151
            }
        );

        testRoom.tiles = addRoom(
            testRoom.tiles,
            RectangleExI{
                Vector2I{
                    0,
                    0
                },
                15,
                15
            }
        );

        testRoom.tiles = addRoom(
            testRoom.tiles,
            RectangleExI{
                -7,
                2,
                7,
                4
            }
        );

        testRoom.tiles = addRoom(
            testRoom.tiles,
            RectangleExI{
                3,
                -5,
                3,
                5
            }
        );

        //* Add walls
        testRoom.tiles = addTiles(
            testRoom.tiles,
            RectangleExI{
                1,
                0,
                1,
                8
            },
            RenderId::WALL,
            true,
            true
        );

        testRoom.tiles = addTiles(
            testRoom.tiles,
            RectangleExI{
                4,
                -5,
                1,
                5
            },
            RenderId::FLOOR
        );

        testRoom.tiles = addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -4,
                3,
                1
            },
            RenderId::FLOOR
        );

        testRoom.tiles = addTiles(
            testRoom.tiles,
            RectangleExI{
                3,
                -2,
                3,
                1
            },
            RenderId::FLOOR
        );

        //* Tiles
        testRoom.tiles.insert(
            Vector2I{
                0,
                -1
            },
            RenderId::WALL,
            true,
            true
        );

        testRoom.tiles.insert(
            Vector2I{
                5,
                6
            },
            RenderId::WALL,
            true,
            true
        );

        testRoom.tiles.insert(
            Vector2I{
                6,
                5
            },
            RenderId::WALL,
            true,
            true
        );

        testRoom.tiles.insert(
            Vector2I{
                5,
                5
            },
            RenderId::WALL,
            true,
            true
        );

        testRoom.tiles.insert(
            Vector2I{
                -6,
                5
            },
            RenderId::FLOOR
        );

        //* Next level trigger
        testRoom.objects.insert(
            Vector2I{
                0,
                0
            },
            "Stairs",
            "Descend",
            RenderId::DESCEND,
            EventId::NEXT_LEVEL
        );

        testRoom.enemies = EnemiesModule::createAtPosition(
            testRoom.enemies,
            testRoom.tiles,
            RenderId::GOBLIN,
            Vector2I{ 3, 0 },
            1
        );

        return testRoom;
    }

    Map createStartRoom()
    {
        Map startRoom{};

        startRoom.tiles = addRoom(
            startRoom.tiles,
            RectangleExI{
                Vector2I{
                    0,
                    0
                },
                15,
                15
            }
        );

        //* Add next level trigger
        startRoom.objects.insert(
            Vector2I{
                0,
                -5
            },
            "Stairs",
            "Descend",
            RenderId::DESCEND,
            EventId::NEXT_LEVEL
        );

        return startRoom;
    }

    Map createGridRooms( int mapLevel )
    {
        Map map{};

        Vector2I roomPosition{ 0, 0 };
        int const roomWidth{ 15 };
        int maxRoomOffset{ ( 2 + mapLevel ) * roomWidth };

        //* Add first room
        map.tiles = addRoom(
            map.tiles,
            RectangleExI{
                roomPosition,
                roomWidth,
                roomWidth
            }
        );

        std::unordered_set<Vector2I> usedPositions{ roomPosition };

        //* Take random direction and add room
        while ( Vector2Sum( roomPosition ) < maxRoomOffset )
        {
            //* (Update) Old room position used for room connection
            Vector2I oldRoomPosition{ roomPosition };

            //* Choose random direction
            Vector2I direction{ Directions::directions[snx::RNG::random( 0, 3 )] };

            //* Update new room position
            roomPosition += Vector2Scale( direction, roomWidth );

            //* Add new room if room position unused
            if ( !usedPositions.contains( roomPosition ) )
            {
                usedPositions.insert( roomPosition );

                map.tiles = addRoom(
                    map.tiles,
                    RectangleExI{
                        roomPosition,
                        roomWidth,
                        roomWidth
                    }
                );
            }

            //* Add connection gap in wall between old and new room
            map.tiles = addTiles(
                map.tiles,
                RectangleExI{
                    oldRoomPosition,
                    roomPosition
                },
                RenderId::FLOOR
            );
        }

        //* Add previous level trigger
        map.objects.insert(
            Vector2I{
                0,
                0
            },
            "Stairs",
            "Ascend",
            RenderId::ASCEND,
            EventId::PREVIOUS_LEVEL
        );

        //* Add next level trigger
        map.objects.insert(
            roomPosition,
            "Stairs",
            "Descend",
            RenderId::DESCEND,
            EventId::NEXT_LEVEL
        );

        //* Add enemies
        map.enemies = EnemiesModule::createForMapLevel(
            map.enemies,
            map.tiles,
            mapLevel
        );

        return map;
    }

    Map createRandomMapType( int mapLevel )
    {
        Map newMap{};

        //* Choose map design
        switch ( 1 ) //* RNG::random(1, 2)
        {
            default:
            case 1:
            {
                newMap = createGridRooms( mapLevel );

                break;
            }
        }

        return newMap;
    }
}
