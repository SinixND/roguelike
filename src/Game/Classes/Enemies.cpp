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

void Enemies::insert(
    TransformComponent const& transform,
    MovementComponent const& movement,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    Vector2I const& tilePosition,
    int scanRange,
    RenderId renderId
)
{
    size_t id{ idManager.requestId() };

    ids.insert( tilePosition, id );
    positions.insert( id, Convert::tileToWorld( tilePosition ) );
    renderIds.insert( id, renderId );
    transforms.insert( id, transform );
    movements.insert( id, movement );
    energies.insert( id, energy );
    healths.insert( id, health );
    damages.insert( id, damage );
    ais.insert( id, AIComponent{ scanRange } );
}

void Enemies::remove( size_t id )
{
    ids.erase( Convert::worldToTile( positions[id] ) );
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
    //* - on map (-> contains id)
    //* - not visible
    //* - not solid
    //* - not occupied by other enemy
    if ( !tiles.ids.contains( tilePosition ) )
    {
        return false;
    }

    size_t tileId{ tiles.ids[tilePosition] };

    if ( !( tiles.visibilityIds[tileId] == VisibilityId::VISIBILE )
         && !tiles.isSolids.contains( tileId )
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

        enemies.insert(
            TransformComponent{},
            MovementComponent{},
            EnergyComponent{
                enemyData.energyMax,
                enemyData.energyRegenBase
            },
            HealthComponent{ enemyData.healthBase },
            DamageComponent{ enemyData.damageBase },
            tilePosition,
            enemyData.scanRange,
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

    bool regenerate( snx::DenseMap<size_t, EnergyComponent>* energiesIO )
    {
        bool isEnemyReady{ false };

        for ( EnergyComponent& energy : *energiesIO )
        {
            if ( !EnergyModule::regenerate( &energy ) )
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
        Vector2* currentPosition{};
        //* Cache current position for later comparison
        Vector2I oldPosition{};

        for ( size_t idx{ 0 }; idx < enemies.transforms.size(); ++idx )
        {
            currentPosition = &enemies.positions.values()[idx];

            oldPosition = Convert::worldToTile( *currentPosition );

            //* Update movement
            //* Update ids_ key if tilePosition changes
            MovementSystem::update(
                enemies.transforms.values()[idx],
                // enemies.movements.values()[i),
                *currentPosition,
                enemies.energies.values()[idx],
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
            if ( energies.values()[idx].state == EnergyComponent::State::READY
                 && ais.values()[idx].turn < turn )
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
            if ( enemies.healths.values()[idx].currentHealth <= 0 )
            {
                enemies.remove( enemies.ids.values()[idx]
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
