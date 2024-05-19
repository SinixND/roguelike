#ifndef IG20240211205246
#define IG20240211205246

#include "Attack.h"
#include "Graphic.h"
#include "LayerID.h"
#include "MapGenerator.h"
#include "MapSize.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "TileMap.h"
#include "Unit.h"
#include "VisibilityID.h"
#include <vector>

class World
{
public:
    MapSize mapSize_{};

    Unit hero{
        PositionComponent(),
        GraphicComponent(
            RenderID::HERO,
            LayerID::OBJECT),
        MovementComponent(5, 50),
        VisibilityID::VISIBLE,
        20,
        AttackComponent(
            1,
            1)};

public:
    auto currentMap() -> TileMap& { return *currentMap_; }
    auto currentMapLevel() -> int { return currentMapLevel_; }

    void increaseMapLevel();
    void decreaseMapLevel();

    auto mapOverlay() -> TileOverlayMap& { return mapOverlay_; }

    auto framedMapOverlay() -> TileOverlayMap& { return framedMapOverlay_; }

private:
    std::vector<TileMap> maps_{MapGenerator::createStartRoom(mapSize_)};
    TileOverlayMap mapOverlay_{};
    TileOverlayMap framedMapOverlay_{};

    TileMap* currentMap_{&maps_.back()};

    int maxMapLevel_{};
    int currentMapLevel_{};

private:
    void addNewMap(int level);
    void setCurrentMap(int level);
};

#endif
