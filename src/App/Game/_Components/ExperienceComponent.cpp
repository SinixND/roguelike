#include "ExperienceComponent.h"
#include "ExperienceSystem.h"

#include <cassert>

namespace ExperienceModule
{
    int getExpValue( int defenderExpLevel, int attackerExpLevel )
    {
        int exp = defenderExpLevel * static_cast<int>( defenderExpLevel / attackerExpLevel );

        assert( !( exp < 0 ) && "Exp is negative" );

        return exp;
    }

    ExperienceComponent const& gainExp(
        ExperienceComponent& experience,
        int foeExpLevel
    )
    {
        //* NOTE: Divide if 1 skillpoint is additive powerincrease
        //* NOTE: Subtract if 1 skillpoint is multiplicative powerincrease
        experience.current += getExpValue( foeExpLevel, experience.level );

        while ( experience.current >= experience.levelUpThreshold )
        {
            experience = ExperienceSystem::levelUp( experience );
        }

        return experience;
    }
}
