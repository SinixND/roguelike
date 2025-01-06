#include "Enemies.h"

#include "AIComponent.h"
#include "Convert.h"
#include "DamageComponent.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "Map.h"
#include "MovementSystem.h"
#include "RNG.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "Tiles.h"
#include "TransformComponent.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

Vector2I getRandomPosition(
    Enemies const& enemies,
    Tiles const& tiles)
{
    RectangleExI const& mapSize{tiles.mapSize};

    Vector2I randomPosition{};

    while (true)
    {
        randomPosition.x = snx::RNG::random(
            mapSize.left(),
            mapSize.right());

        randomPosition.y = snx::RNG::random(
            mapSize.top(),
            mapSize.bottom());

        //* Check if random position is
        //* - on map
        //* - not visible
        //* - not solid
        //* - not occupied by other enemy
        size_t tileId{tiles.ids.contains(randomPosition)};

        if (tileId
            && !(tiles.visibilityIds.at(tileId) == VisibilityId::VISIBILE)
            && !tiles.isSolids.contains(tileId)
            && !enemies.ids.contains(randomPosition))
        {
            return randomPosition;
        }
    }
}

void insert(
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
    enemies.ids.insert(tilePosition, id);
    enemies.positions.insert(id, Convert::tileToWorld(tilePosition));
    enemies.renderIds.insert(id, renderId);
    enemies.transforms.insert(id, transform);
    enemies.movements.insert(id, movement);
    enemies.energies.insert(id, energy);
    enemies.healths.insert(id, health);
    enemies.damages.insert(id, damage);
    enemies.ais.insert(id, AIComponent{scanRange});
}

void EnemiesModule::createSingle(
    Enemies& enemies,
    Map const& map,
    RenderId renderId,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(
            enemies,
            map.tiles);
    }

    size_t enemyId{0};

    if (!enemies.ids.contains(tilePosition))
    {
        enemyId = enemies.idManager.requestId();
    }
    else
    {
        enemyId = enemies.ids.at(tilePosition);
    }

    switch (renderId)
    {
        default:
        case RenderId::GOBLIN:
        {
            insert(
                enemies,
                enemyId,
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

void EnemiesModule::remove(
    Enemies& enemies,
    size_t id)
{
    enemies.ids.erase(Convert::worldToTile(enemies.positions.at(id)));
    enemies.positions.erase(id);
    enemies.renderIds.erase(id);
    enemies.transforms.erase(id);
    enemies.movements.erase(id);
    enemies.energies.erase(id);
    enemies.healths.erase(id);
    enemies.damages.erase(id);
    enemies.ais.erase(id);
}

void EnemiesModule::init(
    Enemies& enemies,
    int mapLevel,
    Map const& map)
{
    while (static_cast<int>(enemies.renderIds.size()) < ((mapLevel + 1) * 5))
    {
        EnemiesModule::createSingle(
            enemies,
            map,
            RenderId::GOBLIN);
    }
}

bool EnemiesModule::regenerate(Enemies& enemies)
{
    bool isEnemyReady{false};

    for (EnergyComponent& energy : enemies.energies)
    {
        if (!EnergyModule::regenerate(energy))
        {
            isEnemyReady = true;
        }
    }

    return isEnemyReady;
}

void EnemiesModule::update(
    Enemies& enemies,
    Vector2 const& heroPosition)
{
    for (size_t idx{0}; idx < enemies.transforms.size(); ++idx)
    {
        //* Update movement
        //* Update ids_ key if tilePosition changes
        MovementSystem::updateHero(
            enemies.transforms.values().at(idx),
            // enemies.movements.values().at(i),
            enemies.positions.values().at(idx),
            enemies.energies.values().at(idx),
            heroPosition);
    }
}

size_t EnemiesModule::getActive(
    snx::SparseSet<EnergyComponent> const& energies,
    snx::SparseSet<AIComponent> const& ais,
    int const turn)
{
    size_t activeEnemyId{0};

    for (size_t idx{0}; idx < energies.size(); ++idx)
    {
        if (energies.values().at(idx).state == EnergyState::READY
            && ais.values().at(idx).turn < turn)
        {
            activeEnemyId = energies.key(idx);
            break;
        }
    }

    return activeEnemyId;
}

void EnemiesModule::replaceDead(
    Enemies& enemies,
    Map const& map)
{
    for (auto id : enemies.healths.keys())
    {
        //* Kill enemy at 0 health
        if (enemies.healths.at(id).currentHealth <= 0)
        {
            RenderId enemyType{enemies.renderIds.at(id)};

            EnemiesModule::remove(
                enemies,
                id);

            //* Spawn new enemy
            EnemiesModule::createSingle(
                enemies,
                map,
                enemyType);

            continue;
        }
    }
}
