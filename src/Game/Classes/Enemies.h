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
struct Tiles;

struct Enemies
{
    snx::DenseMap<Vector2I, size_t> ids{};
    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, TransformComponent> transforms{};
    snx::DenseMap<size_t, MovementComponent> movements{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};

    snx::IdManager idManager{};
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
        snx::DenseMap<size_t, EnergyComponent> const& energies,
        snx::DenseMap<size_t, AIComponent> const& ais,
        int const turn);

    void replaceDead(
        Enemies& enemies,
        Map const& map);
}

#endif
