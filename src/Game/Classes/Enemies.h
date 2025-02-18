#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "MovementComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"
#include <cstddef>

struct Vector2I;
struct Map;
class Tiles;

class Enemies
{
public:
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, TransformComponent> transforms{};
    snx::DenseMap<size_t, MovementComponent> movements{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};

public:
    void insert(
        TransformComponent const& transform,
        MovementComponent const& movement,
        EnergyComponent const& energy,
        HealthComponent const& health,
        DamageComponent const& damage,
        Vector2I const& tilePosition,
        int scanRange,
        RenderId renderId
    );

    void remove( size_t id );

private:
    snx::IdManager idManager{};
};

namespace EnemiesModule
{
    [[nodiscard]]
    Enemies const& createAtPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId enemyId,
        Vector2I tilePosition
    );

    [[nodiscard]]
    Enemies const& createAtRandomPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId enemyId
    );

    [[nodiscard]]
    Enemies const& createForLevel(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    );

    [[nodiscard]]
    Enemies const& remove(
        Enemies& enemies,
        size_t id
    );

    //* Returns if an at least one regeneration was successful aka. one enemy is ready
    bool regenerate( snx::DenseMap<size_t, EnergyComponent>& energiesIO );

    [[nodiscard]]
    Enemies const& updateMovements(
        Enemies& enemies,
        float dt
    );

    //* Returns the id of the first ready enemy
    size_t getActive(
        snx::DenseMap<size_t, EnergyComponent> const& energies,
        snx::DenseMap<size_t, AIComponent> const& ais,
        int const turn
    );

    [[nodiscard]]
    Enemies const& replaceDead(
        Enemies& enemies,
        Tiles const& tiles
    );
}

#endif
