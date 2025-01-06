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
#include "SparseSet.h"
#include "TransformComponent.h"

struct Vector2I;
struct Map;
struct Tiles;

struct Enemies
{
    snx::IdManager idManager{};
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::SparseSet<AIComponent> ais{};
    snx::SparseSet<Vector2> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<TransformComponent> transforms{};
    snx::SparseSet<MovementComponent> movements{};
    snx::SparseSet<EnergyComponent> energies{};
    snx::SparseSet<HealthComponent> healths{};
    snx::SparseSet<DamageComponent> damages{};
};

namespace EnemiesModule
{
    void createSingle(
        Enemies& enemies,
        Map const& map,
        RenderId enemyId,
        bool randomPosition = true,
        Vector2I tilePosition = Vector2I{0, 0});

    void init(
        Enemies& enemies,
        int mapLevel,
        Map const& map);

    void remove(
        Enemies& enemies,
        size_t id);

    bool regenerate(Enemies& enemies);

    void update(
        Enemies& enemies,
        Vector2 const& heroPosition);

    size_t getActive(
        snx::SparseSet<EnergyComponent> const& energies,
        snx::SparseSet<AIComponent> const& ais,
        int const turn);

    void replaceDead(
        Enemies& enemies,
        Map const& map);
}

#endif
