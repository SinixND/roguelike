#include "EnergySystem.h"

namespace EnergySystem
{
    void udpate()
    {
        //* Regenerate energy until a unit becomes ready
        bool isUnitReady{ false };

        //* Regenerate until one unit becomes ready
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
        snx::Debugger::cliLog( "No action left. Regen units.\n" );
#endif
        while ( !isUnitReady )
        {
            hero.energy = EnergyModule::regenerate( hero.energy );
            isUnitReady = !( hero.energy.current < 0 );

            for ( size_t idx{ 0 }; idx < enemies.energies.values().size(); ++idx )
            {
#if defined( DEBUG ) && defined( DEBUG_ENERGY )
                snx::Debugger::cliPrint( "[", idx, "] " );
#endif
                enemies.energies.values()[idx] = EnergyModule::regenerate( enemies.energies.values()[idx] );
                isUnitReady |= !( enemies.energies.values()[idx].current < 0 );
            }
        }

        //* Increment turn when hero is ready
        if ( game.hero.energy.isReady )
        {
#if defined( DEBUG ) && defined( DEBUG_GAME_LOOP )
            snx::Debugger::cliLog( "Hero ready. Next Turn\n\n" );
#endif
            snx::EventDispatcher::notify( EventId::NEXT_TURN );
        }

        game.state = GameState::IDLE;
    }
}
