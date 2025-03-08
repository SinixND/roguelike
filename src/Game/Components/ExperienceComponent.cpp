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
    return lvl * static_cast<int>( ( 1 * pow( lvl, 1.5f ) + BASE_EXPERIENCE ) );
}

namespace ExperienceModule
{
    ExperienceComponent const& levelUp( ExperienceComponent& experience )
    {
        experience.expCurrent -= experience.levelUpThreshold;

        ++experience.expLevel;

        experience.levelUpThreshold = getNewThreshold( experience.expLevel );

        return experience;
    }

    ExperienceComponent const& levelUp(
        ExperienceComponent& experience,
        int expLevel
    )
    {
        experience.expLevel = expLevel;

        experience.levelUpThreshold = getNewThreshold( experience.expLevel );

        return experience;
    }

    ExperienceComponent const& gainExp(
        ExperienceComponent& experience,
        int foeExpLevel
    )
    {
        //* NOTE: Divide if 1 skillpoint is additive powerincrease
        //* NOTE: Subtract if 1 skillpoint is multiplicative powerincrease
        experience.expCurrent += foeExpLevel * static_cast<int>( foeExpLevel / experience.expLevel );

        if ( experience.expCurrent < 0 )
        {
            experience.expCurrent = 0;
        }

        while ( experience.expCurrent > experience.levelUpThreshold )
        {
            experience = levelUp( experience );
        }

        return experience;
    }
}
