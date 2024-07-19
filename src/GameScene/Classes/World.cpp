#include "World.h"

#include "MapGenerator.h"

// Public:
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

// Private:
void World::addNewMap()
{
    maps_.push_back(MapGenerator::createRandomMap(maxMapLevel_ + 1));
}

void World::setCurrentMap(int level)
{
    currentMap_ = &maps_[level];
}
