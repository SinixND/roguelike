#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGenerator.h"
#include <vector>

// World holds maps which are identified by a mapLevel (int)
class World
{
#ifdef DEBUG
    std::vector<Map> maps_{MapGenerator::createTestRoom()};
#else
    std::vector<Map> maps_{MapGenerator::createStartRoom()};
#endif

    Map* currentMap_{&maps_.back()};

    int maxMapLevel_{};
    int currentMapLevel_{};

    void addNewMap();
    void setCurrentMap(int level);

public:
    Map& currentMap();
    int currentMapLevel();

    void increaseMapLevel();
    void decreaseMapLevel();
};

#endif
