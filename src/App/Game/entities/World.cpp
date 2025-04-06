#include "World.h"

#include "MapGeneratorSystem.h"

namespace WorldModule
{
    World const& addNewMap( World& world )
    {
        world.maps_.push_back( MapGeneratorSystem::createRandomMapType( world.maxMapLevel_ ) );

        return world;
    }

    World const& setCurrentMap(
        World& world,
        int mapLevel
    )
    {
        world.currentMap = &world.maps_[mapLevel];

        return world;
    }

    World const& increaseMapLevel( World& world )
    {
        if ( world.currentMapLevel == world.maxMapLevel_ )
        {
            ++world.maxMapLevel_;
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

