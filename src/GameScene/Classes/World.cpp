#include "World.h"

#include "MapHandler.h"
#include "TileMap.h"

// Public:
auto World::currentMap() -> TileMap& { return *currentMap_; }

auto World::currentLevel() -> int { return currentLevel_; }

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
    {
        return;
    }

    --currentLevel_;
    setMap(currentLevel_);
}

auto World::mapOverlay() -> TileOverlayMap& { return mapOverlay_; }

auto World::framedMapOverlay() -> TileOverlayMap& { return framedMapOverlay_; }

// Private:
void World::addNewMap(int level)
{
    maps_.push_back(MapHandler::createNewMap(level));
}

void World::setMap(int level)
{
    currentMap_ = &maps_[level];
}
