#ifndef IG20240211205246
#define IG20240211205246

#include "MapGenerator.h"
#include "TileMap.h"
#include <vector>

class World
{
public:
    auto currentMap() -> TileMap& { return *currentMap_; }
    auto currentMapLevel() -> int { return currentMapLevel_; }

    void increaseMapLevel();
    void decreaseMapLevel();

    auto mapOverlay() -> TileOverlayMap& { return mapOverlay_; }

    auto framedMapOverlay() -> TileOverlayMap& { return framedMapOverlay_; }

private:
    TileOverlayMap mapOverlay_{};
    TileOverlayMap framedMapOverlay_{};

    std::vector<TileMap> maps_{MapGenerator::createStartRoom()};

    TileMap* currentMap_{&maps_.back()};

    int maxMapLevel_{};
    int currentMapLevel_{};

private:
    void addNewMap(int level);
    void setCurrentMap(int level);
};

#endif