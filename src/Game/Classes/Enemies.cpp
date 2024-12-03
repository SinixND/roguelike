#include "Enemies.h"

#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "Map.h"
#include "MovementSystem.h"
#include "PositionComponent.h"
#include "RNG.h"
#include "RenderId.h"
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
    RectangleExI const& mapSize{tiles.mapSize()};

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
        if (
            tiles.positions.contains(randomPosition)
            && !(tiles.visibilityIds.at(randomPosition) == VisibilityId::VISIBILE)
            && !tiles.isSolids.contains(randomPosition)
            && !enemies.ids.contains(randomPosition))
        {
            return randomPosition;
        }
    }
}

void insertEnemy(
    Enemies* enemies,
    size_t id,
    RenderId renderId,
    TransformComponent const& movement,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    int scanRange,
    Vector2I const& tilePosition)
{
    enemies->ids.insert(tilePosition, id);
    enemies->positions.insert(id, PositionComponent{tilePosition});
    enemies->renderIds.insert(id, renderId);
    enemies->transforms.insert(id, movement);
    enemies->energies.insert(id, energy);
    enemies->healths.insert(id, health);
    enemies->damages.insert(id, damage);
    enemies->ais.insert(id, AIComponent{scanRange});
}

void removeEnemy(
    Enemies* enemies,
    size_t id)
{
    enemies->ids.erase(enemies->positions.at(id).tilePosition());
    enemies->positions.erase(id);
    enemies->renderIds.erase(id);
    enemies->transforms.erase(id);
    enemies->energies.erase(id);
    enemies->healths.erase(id);
    enemies->damages.erase(id);
    enemies->ais.erase(id);
}

void createEnemy(
    Enemies* enemies,
    Map const& map,
    RenderId enemyId,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(
            *enemies,
            map.tiles);
    }

    size_t newId{Enemies::idManager_.requestId()};

    switch (enemyId)
    {
        default:
        case RenderId::GOBLIN:
        {
            insertEnemy(
                enemies,
                newId,
                RenderId::GOBLIN,
                TransformComponent{},
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

void initEnemies(
    Enemies* enemies,
    int mapLevel,
    Map const& map)
{
    while (static_cast<int>(enemies->renderIds.size()) < ((mapLevel + 1) * 5))
    {
        createEnemy(
            enemies,
            map,
            RenderId::GOBLIN);
    }
}

bool regenerateAll(Enemies* enemies)
{
    bool isEnemyReady{false};

    for (EnergyComponent& energy : enemies->energies)
    {
        if (!energy.regenerate())
        {
            isEnemyReady = true;
        }
    }

    return isEnemyReady;
}

void updateEnemies(
    Enemies* enemies,
    Map const& map,
    PositionComponent const& heroPosition)
{
    size_t i{0};

    while (i < enemies->ids.values().size())
    {
        //* Kill enemy at 0 health
        if (enemies->healths.values().at(i).currentHealth() <= 0)
        {
            removeEnemy(enemies, enemies->ids.values().at(i));

            //* Spawn new enemy
            createEnemy(enemies, map, RenderId::GOBLIN);

            continue;
        }

        PositionComponent& position{enemies->positions.values().at(i)};

        Vector2I oldPosition{position.tilePosition()};

        //* Update movement
        //* Update ids_ key if tilePosition changes
        if (
            MovementSystem::update(
                enemies->transforms.values().at(i),
                position,
                enemies->energies.values().at(i),
                map,
                heroPosition))
        {
            enemies->ids.changeKey(
                oldPosition,
                position.tilePosition());
        }

        ++i;
    }
}
