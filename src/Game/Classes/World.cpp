#include "World.h"
#include "MapGeneratorSystem.h"

void World::increaseMapLevel()
{
    if (currentMapLevel == maxMapLevel_)
    {
        ++maxMapLevel_;
        addNewMap();
    }

    ++currentMapLevel;
    setCurrentMap(currentMapLevel);
}

void World::decreaseMapLevel()
{
    if (currentMapLevel == 0)
    {
        return;
    }

    --currentMapLevel;
    setCurrentMap(currentMapLevel);
}

void World::addNewMap()
{
    maps_.push_back(MapGeneratorSystem::createRandomMap(maxMapLevel_ + 1));
}

void World::setCurrentMap(int level)
{
    currentMap = &maps_[level];
}
