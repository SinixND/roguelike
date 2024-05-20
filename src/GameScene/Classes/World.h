#ifndef IG20240211205246
#define IG20240211205246

#include "Attack.h"
#include "Event.h"
#include "Graphic.h"
#include "ISubscriber.h"
#include "LayerID.h"
#include "MapGenerator.h"
#include "MapSize.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "TileMap.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
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
    TileMap& currentMap() { return *currentMap_; }
    int currentMapLevel() { return currentMapLevel_; }

    void increaseMapLevel();
    void decreaseMapLevel();

    TileOverlayMap& mapOverlay() { return mapOverlay_; }

    TileOverlayMap& framedMapOverlay() { return framedMapOverlay_; }

    TileMap& tilesToRender() { return tilesToRender_; }
    void initTilesToRender(Camera2D const& camera, RectangleEx const& panelMapExtended);

private:
    std::vector<TileMap> maps_{MapGenerator::createStartRoom(mapSize_)};

    TileMap tilesToRender_{};

    TileOverlayMap mapOverlay_{};
    TileOverlayMap framedMapOverlay_{};

    TileMap* currentMap_{&maps_.back()};

    int maxMapLevel_{};
    int currentMapLevel_{};

private:
    void addNewMap(int level);
    void setCurrentMap(int level);

public:
    class SubUpdateRenderTiles : public ISubscriber
    {
    public:
        void onNotify() override;

        SubUpdateRenderTiles(
            Event event,
            World& world,
            Camera2D& camera,
            RectangleEx& panelMap)
            : ISubscriber(event)
            , world_(world)
            , camera_(camera)
            , panelMap_(panelMap)
        {
        }

    private:
        World& world_;
        Camera2D& camera_;
        RectangleEx& panelMap_;
    };
};

#endif
