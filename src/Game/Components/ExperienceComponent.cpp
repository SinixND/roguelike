#include "ExperienceComponent.h"

#include <cmath>

int getNewThreshold( int level )
{
    //* NOTE: Multiply by current level to kills the scaling threshold
    int lvl = level - 1;

    //* Kills needed per level:
    //* exp(lvl)    = grows exponentially
    //* f'          = grows linearly,
    //* f''         = const;
    // return lvl * static_cast<int>( ( 0.5f * pow( lvl, 2.0f ) + 0.5f * lvl + 10 ) );

    //* Kills needed per level:
    //* exp(lvl)    = grows exponentially
    //* f'          = grows logarithmicly,
    //* f''         = shrinks reciprocal (inverted logarithmic);
    return lvl * static_cast<int>( ( 1 * pow( lvl, 1.5f ) + 10 ) );
}

namespace ExperienceModule
{
    ExperienceComponent const& levelUp( ExperienceComponent& component )
    {
        component.expCurrent -= component.levelUpThreshold;

        ++component.expLevel;

        component.levelUpThreshold = getNewThreshold( component.expLevel );

        return component;
    }

    ExperienceComponent const& levelUp(
        ExperienceComponent& component,
        int expLevel
    )
    {
        component.expLevel = expLevel;

        component.levelUpThreshold = getNewThreshold( component.expLevel );

        return component;
    }

    ExperienceComponent const& gainExp(
        ExperienceComponent& component,
        int foeExpLevel
    )
    {
        //* NOTE: Divide if 1 skillpoint is additive powerincrease
        //* NOTE: Subtract if 1 skillpoint is multiplicative powerincrease
        component.expCurrent += foeExpLevel * static_cast<int>( foeExpLevel / component.expLevel );

        if ( component.expCurrent < 0 )
        {
            component.expCurrent = 0;
        }

        while ( component.expCurrent > component.levelUpThreshold )
        {
            component = levelUp( component );
        }

        return component;
    }
}
