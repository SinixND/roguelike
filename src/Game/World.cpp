#include "World.h"

// Public:
TileMap& World::getCurrentMap() { return currentMap_; }

size_t World::getCurrentLevel() { return currentLevel_; }

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

// Private:
void World::addNewMap(size_t level)
{
    maps_.push_back(generator_.createNewMap(level));
}

void World::setMap(size_t level)
{
    currentMap_ = maps_[level];
}
