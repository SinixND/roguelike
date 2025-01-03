#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "TransformComponent.h"
#include <cstddef>

struct Vector2I;
struct Map;
struct Tiles;

struct Enemies
{
    snx::SparseSet<AIComponent> ais{};
    snx::SparseSet<PositionComponent> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<TransformComponent> transforms{};
    snx::SparseSet<MovementComponent> movements{};
    snx::SparseSet<EnergyComponent> energies{};
    snx::SparseSet<HealthComponent> healths{};
    snx::SparseSet<DamageComponent> damages{};

    snx::IdManager idManager{};
};

void createEnemy(
    Enemies& enemies,
    Tiles const& tiles,
    RenderId renderId,
    bool randomPosition = true,
    Vector2I tilePosition = Vector2I{0, 0});

Enemies initEnemies(
    int mapLevel,
    Map& map);

bool regenerateEnergies(Enemies& enemies);

void updateEnemies(
    Enemies& enemies,
    PositionComponent const& heroPosition);

size_t getActiveEnemy(
    snx::SparseSet<EnergyComponent> const& energies,
    snx::SparseSet<AIComponent> const& ais,
    int turn);

void removeEnemy(
    Enemies& enemies,
    size_t id);

void replaceDeadEnemies(
    Enemies& enemies,
    Map const& map);

#endif
