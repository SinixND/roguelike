#include "Levels.h"

#include "MapGeneratorSystem.h"

void Levels::addNewMap()
{
    maps_.push_back( MapGeneratorSystem::createRandomMapType( maxMapLevel_ ) );
}

void Levels::setCurrentMap( int level )
{
    currentMap = &maps_[level];
}

Levels::Levels()
{
    currentMap = &maps_.back();
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

