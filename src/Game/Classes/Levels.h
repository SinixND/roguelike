#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGeneratorSystem.h"
#include <vector>

/// Level holds maps which are identified by a mapLevel (int)
class Levels
{
public:
    int currentMapLevel{};
    Map* currentMap{};

public:
    Levels();

    void increaseMapLevel();
    void decreaseMapLevel();

private:
    void addNewMap();
    void setCurrentMap( int level );

private:
#if defined( DEBUG )
    std::vector<Map> maps{ MapGeneratorSystem::createTestRoom() };
#else
    std::vector<Map> maps{ MapGeneratorSystem::createStartRoom() };
#endif

    int maxMapLevel{};
};

#endif
