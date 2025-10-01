#include "Enemies.h"

// #define DEBUG_ENERGY

#include "AIComponent.h"
#include "AttributeSystem.h"
#include "AttributesComponent.h"
#include "Convert.h"
#include "DamageComponent.h"
#include "DenseMap.h"
#include "EnemiesData.h"
#include "EnergyComponent.h"
#include "ExperienceComponent.h"
#include "ExperienceSystem.h"
#include "HealthComponent.h"
#include "IdManager.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "RNG.h"
#include "RenderId.h"
#include "Tiles.h"
#include "TransformComponent.h"
#include "VisibilityId.h"
#include "raylibEx.h"
#include <vector>

#if defined( DEBUG ) && defined( DEBUG_ENERGY )
#include "Debugger.h"
#endif

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

    size_t tileId{ tiles.ids.at( tilePosition ) };

    if ( !( tiles.visibilityIds.at( tileId ) == VisibilityId::VISIBILE )
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
        }
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

[[nodiscard]]
Enemies const& insert(
    Enemies& enemies,
    EnergyComponent const& energy,
    HealthComponent const& health,
    DamageComponent const& damage,
    int vitality,
    int strength,
    int defense,
    int agility,
    Vector2I const& tilePosition,
    int scanRange,
    int expLevel,
    RenderId renderId
)
{
    size_t id{ Enemies::idManager.requestId() };

    enemies.ids.insert( tilePosition, id );
    enemies.ais.insert( id, AIComponent{ .scanRange = scanRange } );
    enemies.positions.insert( id, Convert::tileToWorld( tilePosition ) );
    enemies.renderIds.insert( id, renderId );
    enemies.names.insert( id, renderNames.at( renderId ) );
    enemies.energies.insert( id, energy );
    enemies.healths.insert( id, health );
    enemies.damages.insert( id, damage );
    enemies.experiences.insert( id, ExperienceComponent{} );
    enemies.attributes.insert( id, AttributesComponent{
                                       vitality,
                                       strength,
                                       defense,
                                       agility,
                                   } );

    enemies.experiences.at( id ) = ExperienceSystem::levelUpTo(
        enemies.experiences.at( id ),
        expLevel
    );

    return enemies;
}

[[nodiscard]]
Enemies const& remove(
    Enemies& enemies,
    size_t id
)
{
    Enemies::idManager.suspendId( id );

    enemies.ids.erase( Convert::worldToTile( enemies.positions.at( id ) ) );
    enemies.ais.erase( id );
    enemies.positions.erase( id );
    enemies.renderIds.erase( id );
    enemies.names.erase( id );
    enemies.energies.erase( id );
    enemies.healths.erase( id );
    enemies.damages.erase( id );
    enemies.experiences.erase( id );
    enemies.attributes.erase( id );

    enemies.actions.erase( id );
    enemies.attacks.erase( id );
    enemies.moves.erase( id );
    enemies.isIdles.erase( id );

    return enemies;
}

[[nodiscard]]
Enemies const& createAtRandomPosition(
    Enemies& enemies,
    Tiles const& tiles,
    RenderId renderId,
    int mapLevel
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
        tilePosition,
        mapLevel
    );

    return enemies;
}
[[nodiscard]]
Enemies const& updateAllStats( Enemies& enemies )
{
    for ( size_t i{ 0 }; i < enemies.healths.values().size(); ++i )
    {
        AttributeSystem::updateStats(
            enemies.healths[i],
            enemies.energies[i],
            enemies.attributes[i]
        );
    }

    return enemies;
}

namespace EnemiesModule
{
    Enemies const& createAtPosition(
        Enemies& enemies,
        Tiles const& tiles,
        RenderId renderId,
        Vector2I tilePosition,
        int mapLevel
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

        //* Use mapLevel as expLevel for created enemies
        enemies = insert(
            enemies,
            EnergyComponent{},
            HealthComponent{ enemyData.healthBase },
            DamageComponent{ enemyData.damageBase },
            enemyData.vitality,
            enemyData.strength,
            enemyData.defense,
            enemyData.agility,
            tilePosition,
            enemyData.scanRange,
            mapLevel,
            RenderId::GOBLIN
        );

        enemies = updateAllStats( enemies );

        return enemies;
    }

    Enemies const& createForMapLevel(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    )
    {
        size_t const maxEnemies{ static_cast<size_t>( ( mapLevel + 1 ) * 2 ) };

        while ( enemies.renderIds.size() < maxEnemies )
        {
            enemies = createAtRandomPosition(
                enemies,
                tiles,
                RenderId::GOBLIN,
                mapLevel
            );
        }

        return enemies;
    }

    Enemies const& replaceDead(
        Enemies& enemies,
        Tiles const& tiles,
        int mapLevel
    )
    {
        size_t idx{ 0 };

        while ( idx < enemies.ids.values().size() )
        {
            //* Kill enemy at 0 health
            if ( enemies.healths[idx].current > 0 )
            {
                ++idx;

                continue;
            }

            snx::Logger::log( enemies.names[idx] + " died.\n" );

            enemies = remove(
                enemies,
                enemies.ids[idx]
            );

            //* Spawn new enemy
            enemies = createAtRandomPosition(
                enemies,
                tiles,
                RenderId::GOBLIN,
                mapLevel
            );
        }

        return enemies;
    }
}
