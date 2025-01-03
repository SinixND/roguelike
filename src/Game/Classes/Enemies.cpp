#include "Enemies.h"

#include "AIComponent.h"
#include "DamageComponent.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PositionComponent.h"
#include "RNG.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "Tiles.h"
#include "TransformComponent.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <cstddef>

Vector2I getRandomPosition(
    Enemies const& enemies,
    Tiles const& tiles)
{
    RectangleExI const& mapSize{tiles.mapSize};

    PositionComponent randomPosition{Vector2I{}};

    while (true)
    {
        randomPosition.worldPixel.x = snx::RNG::random(
            mapSize.left(),
            mapSize.right());

        randomPosition.worldPixel.y = snx::RNG::random(
            mapSize.top(),
            mapSize.bottom());

        //* Check if random position is
        //* - on map
        //* - not visible
        //* - not solid
        //* - not occupied by other enemy
        size_t id{tiles.positions.contains(PositionComponent{randomPosition})};

        if (id
            && !(tiles.visibilityIds.at(id) == VisibilityId::VISIBILE)
            && !tiles.isSolids.contains(id)
            && !enemies.positions.contains(id))
        {
            return randomPosition.tilePosition();
        }
    }
}

void insertEnemy(
    Enemies& enemies,
    size_t id,
    RenderId renderId,
    TransformComponent const& transform,
    MovementComponent const& movement,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    int scanRange,
    Vector2I const& tilePosition)
{
    enemies.positions.insert(id, PositionComponent{tilePosition});
    enemies.renderIds.insert(id, renderId);
    enemies.transforms.insert(id, transform);
    enemies.movements.insert(id, movement);
    enemies.energies.insert(id, energy);
    enemies.healths.insert(id, health);
    enemies.damages.insert(id, damage);
    enemies.ais.insert(id, AIComponent{scanRange});
}

void removeEnemy(
    Enemies& enemies,
    size_t id)
{
    enemies.positions.erase(id);
    enemies.renderIds.erase(id);
    enemies.transforms.erase(id);
    enemies.movements.erase(id);
    enemies.energies.erase(id);
    enemies.healths.erase(id);
    enemies.damages.erase(id);
    enemies.ais.erase(id);

    enemies.idManager.suspendId(id);
}

void createEnemy(
    Enemies& enemies,
    Tiles const& tiles,
    RenderId renderId,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(
            enemies,
            tiles);
    }

    size_t id{enemies.idManager.requestId()};

    switch (renderId)
    {
        default:
        case RenderId::GOBLIN:
        {
            insertEnemy(
                enemies,
                id,
                RenderId::GOBLIN,
                TransformComponent{},
                MovementComponent{},
                EnergyComponent{
                    EnemyData::goblinEnergyMax,
                    EnemyData::goblinEnergyRegenBase},
                HealthComponent{EnemyData::goblinHealthBase},
                DamageComponent{EnemyData::goblinDamageBase},
                EnemyData::goblinScanRange,
                tilePosition);

            break;
        }
    }
}

Enemies initEnemies(
    int mapLevel,
    Map& map)
{
    Enemies enemies{};

    while (static_cast<int>(enemies.renderIds.size()) < ((mapLevel + 1) * 5))
    {
        createEnemy(
            enemies,
            map.tiles,
            RenderId::GOBLIN);
    }

    return enemies;
}

bool regenerateEnergies(Enemies& enemies)
{
    bool isEnemyReady{false};

    for (EnergyComponent& energy : enemies.energies)
    {
        if (!energy.regenerate())
        {
            isEnemyReady = true;
        }
    }

    return isEnemyReady;
}

void updateEnemies(
    Enemies& enemies,
    PositionComponent const& heroPosition)
{
    for (size_t idx{0}; idx < enemies.transforms.size(); ++idx)
    {
        size_t id{enemies.transforms.key(idx)};

        //* Update movement
        //* Update ids_ key if tilePosition changes
        MovementSystem::update(
            enemies.transforms.at(id),
            enemies.movements.at(id),
            enemies.positions.at(id),
            enemies.energies.at(id),
            heroPosition);
    }
}

size_t getActiveEnemy(
    snx::SparseSet<EnergyComponent> const& energies,
    snx::SparseSet<AIComponent> const& ais,
    int turn)
{
    size_t activeEnemyId{0};

    for (size_t idx{0}; idx < energies.size(); ++idx)
    {
        size_t id{energies.key(idx)};

        if (energies.at(id).isReady()
            && ais.at(id).turn < turn)
        {
            activeEnemyId = id;
            break;
        }
    }

    return activeEnemyId;
}

void replaceDeadEnemies(
    Enemies& enemies,
    Map const& map)
{
    size_t idx{0};

    while (idx < enemies.healths.size())
    {
        size_t id{enemies.healths.key(idx)};

        //* Kill enemy at 0 health
        if (enemies.healths.at(id).currentHealth() <= 0)
        {
            removeEnemy(
                enemies,
                id);

            //* Spawn new enemy
            createEnemy(
                enemies,
                map.tiles,
                RenderId::GOBLIN);

            continue;
        }

        ++idx;
    }
}
