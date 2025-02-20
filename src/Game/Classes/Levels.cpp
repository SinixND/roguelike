#include "Levels.h"

#include "MapGeneratorSystem.h"

Levels::Levels()
{
    currentMap = &maps.back();
}

void Levels::increaseMapLevel()
{
    if ( currentMapLevel == maxMapLevel_ )
    {
        ++maxMapLevel_;
        addNewMap();
    }

    ++currentMapLevel;

    setCurrentMap( currentMapLevel );
}

void Levels::decreaseMapLevel()
{
    if ( currentMapLevel == 0 )
    {
        return;
    }

    --currentMapLevel;

    setCurrentMap( currentMapLevel );
}

void Levels::addNewMap()
{
    maps.push_back( MapGeneratorSystem::createRandomMapType( maxMapLevel_ ) );
}

void Levels::setCurrentMap( int level )
{
    currentMap = &maps[level];
}

