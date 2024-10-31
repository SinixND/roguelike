#include "EnemySoA.h"
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
#include "TileSoA.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

Vector2I EnemySoA::getRandomPosition(TileSoA const& tiles)
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

void EnemySoA::insert(
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

void EnemySoA::remove(size_t id)
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

void EnemySoA::create(
    Map const& map,
    RenderID enemyId,
    bool randomPosition,
    Vector2I tilePosition)
{
    //* Allow creating enemy at specified position
    if (randomPosition)
    {
        tilePosition = getRandomPosition(map.tiles);
    }

    size_t newID{idManager_.requestId()};

    switch (enemyId)
    {
        default:
        case RenderID::GOBLIN:
        {
            insert(
                newID,
                RenderID::GOBLIN,
                MovementComponent{20 * EnemyData::GOBLIN_BASE_AGILITY},
                EnergyComponent{EnemyData::GOBLIN_BASE_AGILITY},
                HealthComponent{EnemyData::GOBLIN_BASE_HEALTH},
                DamageComponent{EnemyData::GOBLIN_BASE_DAMAGE},
                EnemyData::GOBLIN_SCAN_RANGE,
                tilePosition);

            break;
        }
    }
}

void EnemySoA::init(
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

bool EnemySoA::regenerate()
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

void EnemySoA::update(
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
