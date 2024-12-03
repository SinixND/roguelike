#include "World.h"
#include "MapGeneratorSystem.h"

void World::increaseMapLevel()
{
    if (currentMapLevel_ == maxMapLevel_)
    {
        ++maxMapLevel_;
        addNewMap();
    }

    ++currentMapLevel_;
    setCurrentMap(currentMapLevel_);
}

void World::decreaseMapLevel()
{
    if (currentMapLevel_ == 0)
    {
        return;
    }

    --currentMapLevel_;
    setCurrentMap(currentMapLevel_);
}

int World::currentMapLevel() const
{
    return currentMapLevel_;
}

void World::addNewMap()
{
    maps_.push_back(MapGeneratorSystem::createRandomMap(maxMapLevel_ + 1));
}

void World::setCurrentMap(int level)
{
    currentMap = &maps_[level];
}
