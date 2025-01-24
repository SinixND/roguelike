#include "Enemies.h"

#include "AIComponent.h"
#include "Convert.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnemiesData.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "RNG.h"
#include "RenderId.h"
#include "Tiles.h"
#include "TransformComponent.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

void Enemies::insertSingle(
    TransformComponent const& transform,
    MovementComponent const& movement,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    int scanRange,
    Vector2I const& tilePosition,
    RenderId renderId
)
{
    size_t enemyId{ idManager.requestId() };

    ids.insert( tilePosition, enemyId );
    positions.insert( enemyId, Convert::tileToWorld( tilePosition ) );
    renderIds.insert( enemyId, renderId );
    transforms.insert( enemyId, transform );
    movements.insert( enemyId, movement );
    energies.insert( enemyId, energy );
    healths.insert( enemyId, health );
    damages.insert( enemyId, damage );
    ais.insert( enemyId, AIComponent{ scanRange } );
}

void Enemies::remove( size_t id )
{
    ids.erase( Convert::worldToTile( positions.at( id ) ) );
    positions.erase( id );
    renderIds.erase( id );
    transforms.erase( id );
    movements.erase( id );
    energies.erase( id );
    healths.erase( id );
    damages.erase( id );
    ais.erase( id );
}

bool isSpawnPositionValid(
    Tiles const& tiles,
    snx::DenseMap<Vector2I, size_t> const& enemiesIds,
    Vector2I const& tilePosition
)
{
    //* Check if random position is
    //* - on map
    //* - not visible
    //* - not solid
    //* - not occupied by other enemy
    if ( tiles.positions.contains( tilePosition )
         && !( tiles.visibilityIds.at( tilePosition ) == VisibilityId::VISIBILE )
         && !tiles.isSolids.contains( tilePosition )
         && !enemiesIds.contains( tilePosition ) )
    {
        return true;
    }

    return false;
}

Vector2I getRandomPosition(
    Tiles const& tiles,
    snx::DenseMap<Vector2I, size_t> const& enemiesIds
)
{
    RectangleExI const& mapSize{ tiles.mapSize };

    Vector2I randomPosition{};

    while ( true )
    {
        randomPosition.x = snx::RNG::random(
            mapSize.left(),
            mapSize.right()
        );

        randomPosition.y = snx::RNG::random(
            mapSize.top(),
            mapSize.bottom()
        );

        if ( isSpawnPositionValid(
                 tiles,
                 enemiesIds,
                 randomPosition
             ) )
        {
            return randomPosition;
        };
    }
}

EnemiesData::EnemyData getEnemyData( RenderId renderId )
{
    switch ( renderId )
    {
        default:
        case RenderId::GOBLIN:
        {
            return EnemiesData::goblin;
        }
    }
}

namespace EnemiesModule
{
    Enemies const& createAtPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId renderId,
        Vector2I tilePosition
    )
    {
        if ( !isSpawnPositionValid(
                 tiles,
                 enemies.ids,
                 tilePosition
             ) )
        {
            return enemies;
        }

        EnemiesData::EnemyData enemyData{ getEnemyData( renderId ) };

        enemies.insertSingle(
            TransformComponent{},
            MovementComponent{},
            EnergyComponent{
                enemyData.energyMax,
                enemyData.energyRegenBase
            },
            HealthComponent{ enemyData.healthBase },
            DamageComponent{ enemyData.damageBase },
            enemyData.scanRange,
            tilePosition,
            RenderId::GOBLIN
        );

        return enemies;
    }

    Enemies const& createAtRandomPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId renderId
    )
    {
        Vector2I tilePosition = getRandomPosition(
            tiles,
            enemies.ids
        );

        enemies = createAtPosition(
            enemies,
            tiles,
            renderId,
            tilePosition
        );

        return enemies;
    }

    Enemies const& createForLevel(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    )
    {
        size_t const maxEnemies{ static_cast<size_t>( ( mapLevel + 1 ) * 3 ) };

        while ( enemies.renderIds.size() < maxEnemies )
        {
            enemies = createAtRandomPosition(
                enemies,
                tiles,
                RenderId::GOBLIN
            );
        }

        return enemies;
    }

    bool regenerate( snx::DenseMap<size_t, EnergyComponent>* energies )
    {
        bool isEnemyReady{ false };

        for ( EnergyComponent& energy : *energies )
        {
            if ( !EnergyModule::regenerate( energy ) )
            {
                isEnemyReady = true;
            }
        }

        return isEnemyReady;
    }

    Enemies const& updateMovements(
        Enemies& enemies,
        Vector2 const& heroPosition,
        float dt
    )
    {
        //* Cache current position for later comparison
        Vector2* currentPosition{};
        Vector2I oldPosition{};

        for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
        {
            currentPosition = &enemies.positions.values().at( idx );

            oldPosition = Convert::worldToTile( *currentPosition );

            //* Update movement
            //* Update ids_ key if tilePosition changes
            MovementSystem::update(
                enemies.transforms.values().at( idx ),
                // enemies.movements.values().at(i),
                *currentPosition,
                enemies.energies.values().at( idx ),
                heroPosition,
                dt
            );

            //* Update position info
            if ( oldPosition != Convert::worldToTile( *currentPosition ) )
            {
                enemies.ids.changeKey(
                    oldPosition,
                    Convert::worldToTile( *currentPosition )
                );
            }
        }

        return enemies;
    }

    size_t getActive(
        snx::DenseMap<size_t, EnergyComponent> const& energies,
        snx::DenseMap<size_t, AIComponent> const& ais,
        int const turn
    )
    {
        size_t activeEnemyId{ 0 };

        for ( size_t idx{ 0 }; idx < energies.size(); ++idx )
        {
            if ( energies.values().at( idx ).state == EnergyState::READY
                 && ais.values().at( idx ).turn < turn )
            {
                activeEnemyId = energies.key( idx );
                break;
            }
        }

        return activeEnemyId;
    }

    Enemies const& replaceDead(
        Enemies& enemies,
        Tiles const& tiles
    )
    {
        size_t idx{ 0 };

        while ( idx < enemies.ids.values().size() )
        {
            //* Kill enemy at 0 health
            if ( enemies.healths.values().at( idx ).currentHealth <= 0 )
            {
                enemies.remove( enemies.ids.values().at( idx )
                );

                //* Spawn new enemy
                enemies = createAtRandomPosition(
                    enemies,
                    tiles,
                    RenderId::GOBLIN
                );

                continue;
            }

            ++idx;
        }

        return enemies;
    }
}
