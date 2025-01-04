#include "World.h"

#include "MapGeneratorSystem.h"

void addNewMap(World& world)
{
    world.maps.push_back(MapGeneratorSystem::createRandomMap(world.maxMapLevel + 1));
}

void setCurrentMap(World& world, int level)
{
    world.currentMap = &world.maps[level];
}

namespace WorldModule
{
    void increaseMapLevel(World& world)
    {
        if (world.currentMapLevel == world.maxMapLevel)
        {
            ++world.maxMapLevel;
            addNewMap(world);
        }

        ++world.currentMapLevel;

        setCurrentMap(
            world,
            world.currentMapLevel);
    }

    void decreaseMapLevel(World& world)
    {
        if (world.currentMapLevel == 0)
        {
            return;
        }

        --world.currentMapLevel;

        setCurrentMap(
            world,
            world.currentMapLevel);
    }
}
