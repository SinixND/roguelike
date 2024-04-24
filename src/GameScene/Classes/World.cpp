#include "World.h"

#include "MapGenerator.h"

// Public:
void World::increaseMapLevel()
{
    if (currentMapLevel_ == maxMapLevel_)
    {
        ++maxMapLevel_;
        addNewMap(maxMapLevel_);
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

// Private:
void World::addNewMap(int level)
{
    maps_.push_back(MapGenerator::createRandomMap(level));
}

void World::setCurrentMap(int level)
{
    currentMap_ = &maps_[level];
}
