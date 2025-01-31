#include "World.h"

#include "MapGeneratorSystem.h"

World::World()
{
    currentMapPtr = &maps.back();
}

void World::increaseMapLevel()
{
    if ( currentMapLevel == maxMapLevel )
    {
        ++maxMapLevel;
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

void World::addNewMap()
{
    maps.push_back( MapGeneratorSystem::createRandomMap( maxMapLevel + 1 ) );
}

void World::setCurrentMap( int level )
{
    currentMapPtr = &maps[level];
}

