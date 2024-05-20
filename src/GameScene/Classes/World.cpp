#include "World.h"

#include "MapGenerator.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "raylibEx.h"
#include <raylib.h>

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

void World::initTilesToRender(Camera2D const& camera, RectangleEx const& panelMapExtended)
{
    tilesToRender_ = TileMapFilters::filterTilesToRender(*currentMap_, camera, panelMapExtended);
}

// Private:
void World::addNewMap(int level)
{
    maps_.push_back(MapGenerator::createRandomMap(level, mapSize_));
}

void World::setCurrentMap(int level)
{
    currentMap_ = &maps_[level];
}

void World::SubUpdateRenderTiles::onNotify()
{
    world_.initTilesToRender(camera_, panelMap_);
}