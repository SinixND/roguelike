#include "Enemies.h"
#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "Map.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "PositionComponent.h"
#include "RNG.h"
#include "RenderID.h"
#include "Tiles.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

Vector2I Enemies::getRandomPosition(Tiles const& tiles)
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
            && !(tiles.visibilityIDs.at(randomPosition) == VisibilityID::VISIBILE)
            && !tiles.isSolid(randomPosition)
            && !this->ids.contains(randomPosition))
        {
            return randomPosition;
        }
    }
}

void Enemies::insert(
    size_t id,
    RenderID renderID,
    MovementComponent const& movement,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    int scanRange,
    Vector2I const& tilePosition)
{
    ids.insert(tilePosition, id);
    positions.insert(id, PositionComponent{tilePosition});
    renderIDs.insert(id, renderID);
    movements.insert(id, movement);
    energies.insert(id, energy);
    healths.insert(id, health);
    damages.insert(id, damage);
    ais.insert(id, AIComponent{scanRange});
}

void Enemies::remove(size_t id)
{
    ids.erase(positions.at(id).tilePosition());
    positions.erase(id);
    renderIDs.erase(id);
    movements.erase(id);
    energies.erase(id);
    healths.erase(id);
    damages.erase(id);
    ais.erase(id);
}

void Enemies::create(
    Map const& map,
    RenderID enemyID,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(map.tiles);
    }

    size_t newID{idManager_.requestId()};

    switch (enemyID)
    {
        default:
        case RenderID::GOBLIN:
        {
            insert(
                newID,
                RenderID::GOBLIN,
                MovementComponent{20 * EnemyData::goblinAgilityBase},
                EnergyComponent{EnemyData::goblinAgilityBase},
                HealthComponent{EnemyData::goblinHealthBase},
                DamageComponent{EnemyData::goblinDamageBase},
                EnemyData::goblinScanRange,
                tilePosition);

            break;
        }
    }
}

void Enemies::init(
    int mapLevel,
    Map const& map)
{
    while (static_cast<int>(renderIDs.size()) < ((mapLevel + 1) * 5))
    {
        create(
            map,
            RenderID::GOBLIN);
    }
}

bool Enemies::regenerate()
{
    bool isEnemyReady{false};

    for (EnergyComponent& energy : energies)
    {
        if (energy.regenerate())
        {
            isEnemyReady = true;
        }
    }

    return isEnemyReady;
}

void Enemies::update(
    Map const& map,
    PositionComponent const& heroPosition)
{
    size_t i{0};

    while (i < ids.values().size())
    {
        //* Kill enemy at 0 health
        if (healths.values().at(i).currentHealth() <= 0)
        {
            remove(ids.values().at(i));

            //* Spawn new enemy
            create(map, RenderID::GOBLIN);

            continue;
        }

        PositionComponent& position{positions.values().at(i)};

        Vector2I oldPosition{position.tilePosition()};

        //* Update ids_ key if tilePosition changes
        if (
            MovementSystem::update(
                movements.values().at(i),
                position,
                energies.values().at(i),
                map,
                heroPosition))
        {
            ids.changeKey(
                oldPosition,
                position.tilePosition());
        }

        ++i;
    }
}
