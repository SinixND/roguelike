#include "Enemies.h"

#include "AIComponent.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnemyData.h"
#include "EnergyComponent.h"
#include "GameCamera.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "Map.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "PathfinderSystem.h"
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
                MovementComponent{20 * EnemyData::GOBLIN_BASE_AGILITY},
                EnergyComponent{EnemyData::GOBLIN_BASE_AGILITY},
                HealthComponent{10},
                DamageComponent{1},
                EnemyData::GOBLIN_SCAN_RANGE,
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

    for (size_t const& enemyId : ids.values())
    {
        if (energies.at(enemyId).regenerate())
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
    //* TODO: consider "day" of last update in loop to handle dying enemies!
    for (size_t const& enemyId : ids.values())
    {
        PositionComponent& position{positions.at(enemyId)};

        Vector2I oldPosition{position.tilePosition()};

        //* Update ids_ key if tilePosition changes
        if (
            MovementSystem::update(
                movements.at(enemyId),
                position,
                energies.at(enemyId),
                map,
                heroPosition))
        {
            ids.changeKey(
                oldPosition,
                position.tilePosition());
        }
    }
}
