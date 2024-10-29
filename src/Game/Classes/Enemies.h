#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "RenderID.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <cstddef>

struct Map;

//* SoA
//* Its the clients responsibility to avoid desync caused by individual size modifications of DenseMaps (eg. insert, erase). Use member functions instead
struct Enemies
{
    snx::DenseMap<Vector2I, size_t> ids{};
    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, PositionComponent> positions{};
    snx::DenseMap<size_t, RenderID> renderIDs{};
    snx::DenseMap<size_t, MovementComponent> movements{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};

private:
    snx::IdManager idManager_{};

public:
    void create(
        Map const& map,
        RenderID enemyID,
        bool randomPosition = true,
        Vector2I tilePosition = Vector2I{0, 0});

    void init(
        int mapLevel,
        Map const& map);

    bool regenerate();

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
        HealthComponent const& health,
        DamageComponent const& damage,
        int scanRange,
        Vector2I const& enemyPosition);
};

#endif
