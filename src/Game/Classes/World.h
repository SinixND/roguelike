#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGeneratorSystem.h"
#include <vector>

//* World holds maps which are identified by a mapLevel (int)
class World
{
public:
    int currentMapLevel{};
    Map* currentMapPtr{};

public:
    World();

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
