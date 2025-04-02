#include "World.h"

#include "MapGeneratorSystem.h"

void World::addNewMap()
{
    maps_.push_back( MapGeneratorSystem::createRandomMapType( maxMapLevel_ ) );
}

void World::setCurrentMap( int mapLevel )
{
    currentMap = &maps_[mapLevel];
}

World::World()
{
    currentMap = &maps_.back();
}

void World::increaseMapLevel()
{
    if ( currentMapLevel == maxMapLevel_ )
    {
        ++maxMapLevel_;
        addNewMap();
    }

    ++currentMapLevel;

    setCurrentMap( currentMapLevel );
}

void World::decreaseMapLevel()
{
    if ( currentMapLevel == 0 )
    {
        return;
    }

    --currentMapLevel;

    setCurrentMap( currentMapLevel );
}

