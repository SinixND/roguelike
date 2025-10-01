#ifndef IG20240909134010
#define IG20240909134010

#include "AIComponent.h"
#include "ActionId.h"
#include "AttackComponent.h"
#include "AttributesComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EmptyComponent.h"
#include "EnergyComponent.h"
#include "ExperienceComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "MoveComponent.h"
#include "MovementComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"
#include <cstddef>

struct Vector2I;
struct Map;
struct Tiles;

struct Enemies
{
    static inline snx::IdManager idManager{};

    //* Persistent components
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::DenseMap<size_t, AIComponent> ais{};
    snx::DenseMap<size_t, Vector2> positions{};
    snx::DenseMap<size_t, RenderId> renderIds{};
    snx::DenseMap<size_t, std::string> names{};
    snx::DenseMap<size_t, EnergyComponent> energies{};
    snx::DenseMap<size_t, HealthComponent> healths{};
    snx::DenseMap<size_t, DamageComponent> damages{};
    snx::DenseMap<size_t, ExperienceComponent> experiences{};
    snx::DenseMap<size_t, AttributesComponent> attributes{};

    //* Optional components
    snx::DenseMap<size_t, ActionId> actions{};
    snx::DenseMap<size_t, AttackComponent> attacks{};
    snx::DenseMap<size_t, MoveComponent> moves{};
    snx::DenseMap<size_t, EmptyComponent> isIdles{};
};

namespace EnemiesModule
{
    [[nodiscard]]
    Enemies const& createAtPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId renderId,
        Vector2I tilePosition,
        int mapLevel
    );

    [[nodiscard]]
    Enemies const& createForMapLevel(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    );

    [[nodiscard]]
    Enemies const& replaceDead(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    );
}

#endif
