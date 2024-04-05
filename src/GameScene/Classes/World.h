#ifndef IG20240211205246
#define IG20240211205246

#include "MapHandler.h"
#include "TileMap.h"
#include <vector>

class World
{
public:
    TileMap& currentMap();
    int currentLevel();

    void increaseLevel();
    void decreaseLevel();

    TileOverlayMap& mapOverlay();
    TileOverlayMap& framedMapOverlay();

private:
    int maxLevel_{};
    int currentLevel_{};

    std::vector<TileMap> maps_{MapHandler::createNewMap(maxLevel_)};
    TileMap* currentMap_{&maps_.back()};

    TileOverlayMap mapOverlay_{};
    TileOverlayMap framedMapOverlay_{};

private:
    void addNewMap(int level);
    void setMap(int level);
};

#endif