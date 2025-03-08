#include "LevelUpSystem.h"

#include "ExperienceComponent.h"
#include <cassert>
#include <cmath>

int getNewThreshold( int level )
{
    //* Kills needed per level:
    //* exp(lvl)    = grows exponentially
    //* f'          = grows linearly,
    //* f''         = const;
    // return lvl * static_cast<int>( ( 0.5f * pow( lvl, 2.0f ) + 0.5f * lvl + 10 ) );

    //* Kills needed per level:
    //* exp(lvl)    = grows exponentially
    //* f'          = grows logarithmicly,
    //* f''         = shrinks reciprocal (inverted logarithmic);
    //* NOTE: Multiply whole expression by current level to make kills necessary the scaling threshold
    int lvl = level - 1;

    return lvl * static_cast<int>( ( 1 * pow( lvl, 1.5f ) + BASE_EXPERIENCE ) );
}

namespace LevelUpSystem
{
    ExperienceComponent const& levelUp( ExperienceComponent& experience )
    {
        experience.current -= experience.levelUpThreshold;

        ++experience.level;

        experience.levelUpThreshold = getNewThreshold( experience.level );

        return experience;
    }

    ExperienceComponent const& levelUpTo(
        ExperienceComponent& experience,
        int expLevel
    )
    {
        if ( experience.level >= expLevel )
        {
            return experience;
        }

        experience.level = expLevel;

        experience.levelUpThreshold = getNewThreshold( experience.level );

        return experience;
    }
}

