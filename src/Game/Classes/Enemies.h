#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "DenseMap.h"
#include "EnergyComponent.h"
#include "GameCamera.h"
#include "IdManager.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

//* SoA class
//* Its the clients responsibility to avoid desync caused by individual size modifications of DenseMaps (eg. insert, erase)
class Enemies
{
public:
    snx::IdManager idManager_{};

    std::vector<size_t> ids_{};
    snx::DenseMap<size_t, PositionComponent> positions_{};
    snx::DenseMap<size_t, RenderID> renderIDs_{};
    snx::DenseMap<size_t, MovementComponent> movements_{};
    snx::DenseMap<size_t, EnergyComponent> energies_{};
    snx::DenseMap<size_t, AIComponent> ais_{};

public:
    //* Pass heroPosition twice to get random position
    void create(
        Map const& map,
        RenderID enemyID,
        Vector2I tilePosition = Vector2I{0, 0});

    void init(
        int mapLevel,
        Map const& map);

    bool regenerate();

    //* Checks consecutively, persistant across game loop iterations
    bool checkForAction(
        Map const& map,
        Vector2I const& heroPosition,
        GameCamera const& gameCamera);

    void update(
        Map const& map,
        PositionComponent const& heroPosition);


private:
    Vector2I getRandomPosition(Tiles const& tiles);

    void insert(
        size_t id,
        RenderID renderID,
        MovementComponent const& movement,
        EnergyComponent const& energy,
        int scanRange,
        Vector2I const& enemyPosition);
};

#endif
