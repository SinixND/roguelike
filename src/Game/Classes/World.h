#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGeneratorSystem.h"
#include <vector>

//* World holds maps which are identified by a mapLevel (int)
struct World
{
#if defined(DEBUG)
    std::vector<Map> maps{MapGeneratorSystem::createTestRoom()};
#else
    std::vector<Map> maps{MapGeneratorSystem::createStartRoom()};
#endif

    int maxMapLevel{};
    int currentMapLevel{};

    Map* currentMap{&maps.back()};
};

namespace WorldModule
{
    void increaseMapLevel(World& world);
    void decreaseMapLevel(World& world);
}

#endif
