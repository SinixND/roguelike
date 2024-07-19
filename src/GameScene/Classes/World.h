#ifndef IG20240531191635
#define IG20240531191635

#include "MapGenerator.h"
#include "Tiles.h"
#include <vector>

// World holds maps which are identified by a mapLevel (int)
class World
{
    std::vector<Tiles> maps_{MapGenerator::createStartRoom()};

    // Filtered tiles for efficiency
    Tiles tilesToRender_{};

    Tiles* currentMap_{&maps_.back()};

    int maxMapLevel_{};
    int currentMapLevel_{};

    void addNewMap();
    void setCurrentMap(int level);

public:
    Tiles& currentMap() { return *currentMap_; };
    int currentMapLevel() { return currentMapLevel_; }

    void increaseMapLevel();
    void decreaseMapLevel();

    Tiles& tilesToRender() { return tilesToRender_; }
};

#endif
