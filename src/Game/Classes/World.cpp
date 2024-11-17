#include "World.h"

#include "MapGeneratorSystem.h"
#include <vector>

struct Map;

void increaseMapLevel(World* world)
{
    if (world->currentMapLevel == world->maxMapLevel)
    {
        ++world->maxMapLevel;

        world->maps.push_back(MapGeneratorSystem::createRandomMap(world->maxMapLevel + 1));
    }

    ++world->currentMapLevel;

    world->currentMap = &world->maps[world->currentMapLevel];
}

void decreaseMapLevel(World* world)
{
    if (world->currentMapLevel == 0)
    {
        return;
    }

    --world->currentMapLevel;

    world->currentMap = &world->maps[world->currentMapLevel];
}
