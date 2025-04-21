
#include "EnergySystem.h"

// #define DEBUG_ENERGY_SYSTEM

#include "DenseMap.h"
#include "EmptyComponent.h"
#include "EnergyComponent.h"
#include "EventDispatcher.h"
#include "EventId.h"

[[nodiscard]]
EnergyComponent const& regenerate( EnergyComponent& energy )
{
    //* If not already full
    if ( energy.current < 0 )
    {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
        snx::Debugger::cliPrint( "Regen ", energyIO.current, "+", energyIO.regenRate, "\n" );
#endif

        //* Regen energy until full
        energy.current += energy.regenRate;
    }

    return energy;
}

namespace EnergySystem
{
    void udpate(
        EnergyComponent& heroEnergyIO,
        bool& heroIsReadyOut,
        snx::DenseMap<size_t, EnergyComponent>& enemyEnergiesIO,
        snx::DenseMap<size_t, EmptyComponent>& enemyIsReadiesOut
    )
    {
        //* Regenerate energy until a unit becomes ready
        bool isUnitReady{ false };

        //* Regenerate until one unit becomes ready
        while ( !isUnitReady )
        {
#if defined( DEBUG ) && defined( DEBUG_ENERGY_SYSTEM )
            snx::Debugger::cliLog( "Regenerating units.\n" );
#endif
            heroEnergyIO = regenerate( heroEnergyIO );

            //* If hero is ready
            if ( !( heroEnergyIO.current < 0 ) )
            {
                //* Increment turn when hero is ready
#if defined( DEBUG ) && defined( DEBUG_ENERGY_SYSTEM )
                snx::Debugger::cliLog( "Hero ready. Next Turn.\n\n" );
#endif
                heroIsReadyOut = true;

                snx::EventDispatcher::notify( EventId::NEXT_TURN );

                isUnitReady = true;
            }

            for ( size_t idx{ 0 }; idx < enemyEnergiesIO.size(); ++idx )
            {
#if defined( DEBUG ) && defined( DEBUG_ENERGY_SYSTEM )
                snx::Debugger::cliPrint( "Regen [", idx, "] " );
#endif
                enemyEnergiesIO[idx] = regenerate( enemyEnergiesIO[idx] );

                //* If enemy is ready
                if ( !( enemyEnergiesIO[idx].current < 0 ) )
                {
#if defined( DEBUG ) && defined( DEBUG_ENERGY_SYSTEM )
                    snx::Debugger::cliPrint( ": is ready.\n\n" );
#endif
                    enemyIsReadiesOut.insert( enemyEnergiesIO.key( idx ) );

                    isUnitReady = true;
                }
            }
        }
    }
}
