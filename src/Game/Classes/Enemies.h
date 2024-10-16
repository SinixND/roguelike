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
//* Containers need to stay in sync, therefore they are private and direct getters are provided to prevent individual inserts/deletions
class Enemies
{
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

    std::vector<size_t> const& getIds() const;

    snx::DenseMap<size_t, AIComponent> const& getAIs() const;
    snx::DenseMap<size_t, AIComponent>& getAIs();

    snx::DenseMap<size_t, PositionComponent> const& getPositions() const;
    snx::DenseMap<size_t, PositionComponent>& getPositions();

    snx::DenseMap<size_t, RenderID> const& getRenderIDs() const;
    snx::DenseMap<size_t, RenderID>& getRenderIDs();

    snx::DenseMap<size_t, MovementComponent> const& getMovements() const;
    snx::DenseMap<size_t, MovementComponent>& getMovements();

    snx::DenseMap<size_t, EnergyComponent> const& gitEnergies() const;
    snx::DenseMap<size_t, EnergyComponent>& getEnergies();

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
