#include "ExperienceComponent.h"
#include "LevelUpSystem.h"

#include <cassert>
#include <cmath>

namespace ExperienceModule
{
    int getExpValue( int foeExpLevel, int attackerExpLevel )
    {
        int exp = foeExpLevel * static_cast<int>( foeExpLevel / attackerExpLevel );
        return ( exp < 0 ) ? 0 : exp;
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
            experience = LevelUpSystem::levelUp( experience );
        }

        return experience;
    }
}
