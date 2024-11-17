#include "Enemies.h"

#include "AIComponent.h"
#include "Convert.h"
#include "DamageComponent.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "Map.h"
#include "MovementSystem.h"
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
        if (
            tiles.positions.contains(randomPosition)
            && !(tiles.visibilityIds.at(randomPosition) == VisibilityId::VISIBILE)
            && !tiles.solids.contains(randomPosition)
            && !enemies.ids.contains(randomPosition))
        {
            return randomPosition;
        }
    }
}

void insert(
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
    enemies->positions.insert(id, Vector2{Convert::tileToWorld(tilePosition)});
    enemies->renderIds.insert(id, renderId);
    enemies->movements.insert(id, movement);
    enemies->energies.insert(id, energy);
    enemies->healths.insert(id, health);
    enemies->damages.insert(id, damage);
    enemies->ais.insert(id, AIComponent{scanRange});
}

void remove(
    Enemies* enemies,
    size_t id)
{
    enemies->ids.erase(Convert::worldToTile(enemies->positions.at(id)));
    enemies->positions.erase(id);
    enemies->renderIds.erase(id);
    enemies->movements.erase(id);
    enemies->energies.erase(id);
    enemies->healths.erase(id);
    enemies->damages.erase(id);
    enemies->ais.erase(id);
}

void ModuleEnemies::create(
    Enemies* enemies,
    Tiles const& tiles,
    RenderId enemyId,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(
            *enemies,
            tiles);
    }

    switch (enemyId)
    {
        default:
        case RenderId::GOBLIN:
        {
            insert(
                enemies,
                Enemies::idManager.requestId(),
                RenderId::GOBLIN,
                TransformComponent{EnemyData::goblinSpeed},
                EnergyComponent{EnemyData::goblinAgilityBase},
                HealthComponent{EnemyData::goblinHealthBase, EnemyData::goblinHealthBase},
                DamageComponent{EnemyData::goblinDamageBase},
                EnemyData::goblinScanRange,
                tilePosition);

            break;
        }
    }
}

void ModuleEnemies::createEnemies(
    Enemies* enemies,
    int mapLevel,
    Tiles const& tiles)
{
    while (static_cast<int>(enemies->renderIds.size()) < ((mapLevel + 1) * 5))
    {
        ModuleEnemies::create(
            enemies,
            tiles,
            RenderId::GOBLIN);
    }
}

void ModuleEnemies::updateEnemies(
    Enemies* enemies,
    Map const& map,
    Vector2 const& heroPosition)
{
    size_t i{0};

    while (i < enemies->ids.values().size())
    {
        //* Kill enemy at 0 health
        if (enemies->healths.values().at(i).currentHealth <= 0)
        {
            remove(
                enemies,
                enemies->ids.values().at(i));

            //* Spawn new enemy
            ModuleEnemies::create(
                enemies,
                map.tiles,
                RenderId::GOBLIN);

            continue;
        }

        Vector2& position{enemies->positions.values().at(i)};

        Vector2I oldPosition{Convert::worldToTile(position)};

        //* Update ids_ key if tilePosition changes
        if (
            MovementSystem::update(
                enemies->movements.values().at(i),
                position,
                enemies->energies.values().at(i),
                map,
                heroPosition))
        {
            enemies->ids.changeKey(
                oldPosition,
                Convert::worldToTile(position));
        }

        ++i;
    }
}
