#include "World.h"

#include "MapHandler.h"
#include "TileMap.h"
#include <cstddef>

// Public:
TileMap& World::currentMap() { return *currentMap_; }

int World::currentLevel() { return currentLevel_; }

void World::increaseLevel()
{
    if (currentLevel_ == maxLevel_)
    {
        ++maxLevel_;
        addNewMap(maxLevel_);
    }

    ++currentLevel_;
    setMap(currentLevel_);
}

void World::decreaseLevel()
{
    if (currentLevel_ == 0)
        return;

    --currentLevel_;
    setMap(currentLevel_);
}

TileMap& World::mapOverlay() { return mapOverlay_; }

TileMap& World::framedMapOverlay() { return framedMapOverlay_; }

// Private:
void World::addNewMap(int level)
{
    maps_.push_back(MapHandler::createNewMap(level));
}

void World::setMap(int level)
{
    currentMap_ = &maps_[level];
}
