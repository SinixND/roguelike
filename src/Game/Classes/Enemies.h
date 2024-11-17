#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "RenderId.h"
#include "TransformComponent.h"
#include <cstddef>

struct Vector2I;
struct Map;
struct Tiles;

struct Enemies
{
    static inline snx::IdManager idManager{};

    snx::DenseMap<Vector2I, size_t> ids{};
    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, TransformComponent> movements{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};
};

namespace ModuleEnemies
{
    void create(
        Enemies* enemies,
        Tiles const& tiles,
        RenderId enemyId,
        bool randomPosition = true,
        Vector2I tilePosition = Vector2I{0, 0});

    void createEnemies(
        Enemies* enemies,
        int mapLevel,
        Tiles const& tiles);

    void updateEnemies(
        Enemies* enemies,
        Map const& map,
        Vector2 const& heroPosition);
}

#endif
