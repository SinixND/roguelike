#include "World.h"

#include "Map.h"
#include "MapGenerator.h"

Map const& World::currentMap() const
{
    return *currentMap_;
}
Map& World::currentMap()
{
    return *currentMap_;
}

int World::currentMapLevel() const
{
    return currentMapLevel_;
}

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

void World::addNewMap()
{
    maps_.push_back(MapGenerator::createRandomMap(maxMapLevel_ + 1));
}

void World::setCurrentMap(int level)
{
    currentMap_ = &maps_[level];
}
