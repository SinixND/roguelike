#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGeneratorSystem.h"
#include <vector>

//* World holds maps which are identified by a mapLevel (int)
class World
{
#if defined(DEBUG)
    std::vector<Map> maps_{MapGeneratorSystem::createTestRoom()};
#else
    std::vector<Map> maps_{MapGeneratorSystem::createStartRoom()};
#endif

    int maxMapLevel_{};

public:
    Map* currentMap{&maps_.back()};
    int currentMapLevel{};

public:
    void increaseMapLevel();
    void decreaseMapLevel();

private:
    void addNewMap();
    void setCurrentMap(int level);
};

#endif
