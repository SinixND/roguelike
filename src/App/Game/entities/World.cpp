#include "World.h"

#include "MapGeneratorSystem.h"

namespace WorldModule
{
    World const& addNewMap( World& world )
    {
        world.maps.push_back( MapGeneratorSystem::createRandomMapType( world.maxMapLevel ) );

        return world;
    }

    World const& setCurrentMap(
        World& world,
        int mapLevel
    )
    {
        world.currentMap = &world.maps[mapLevel];

        return world;
    }

    World const& increaseMapLevel( World& world )
    {
        if ( world.currentMapLevel == world.maxMapLevel )
        {
            ++world.maxMapLevel;
            world = addNewMap( world );
        }

        ++world.currentMapLevel;

        world = setCurrentMap(
            world,
            world.currentMapLevel
        );

        return world;
    }

    World const& decreaseMapLevel( World& world )
    {
        if ( world.currentMapLevel == 0 )
        {
            return world;
        }

        --world.currentMapLevel;

        world = setCurrentMap(
            world,
            world.currentMapLevel
        );

        return world;
    }
}

