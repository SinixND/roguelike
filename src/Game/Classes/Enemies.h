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
#include "RenderId.h"
#include "TransformComponent.h"
#include <cstddef>

struct Vector2I;
struct Map;
class Tiles;

struct Enemies
{
    static inline snx::IdManager idManager_{};

    snx::DenseMap<Vector2I, size_t> ids{};
    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, PositionComponent> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, TransformComponent> transforms{};
    snx::DenseMap<size_t, MovementComponent> movements{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};
};

void createEnemy(
    Enemies& enemies,
    Map const& map,
    RenderId enemyId,
    bool randomPosition = true,
    Vector2I tilePosition = Vector2I{0, 0});

void initEnemies(
    Enemies& enemies,
    int mapLevel,
    Map const& map);

bool regenerateAll(Enemies& enemies);

void updateEnemies(
    Enemies& enemies,
    Map const& map,
    PositionComponent const& heroPosition);

#endif
