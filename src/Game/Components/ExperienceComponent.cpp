#include "ExperienceComponent.h"

#include <cmath>

int getNewThreshold( int level )
{
    return static_cast<int>( ( 0.5f * pow( level, 2.0f ) + 0.5f * level + 10 ) * level );
    // return 0.5f * pow(level, 2.0f) + 0.5f * level + 10;
}

namespace ExperienceModule
{
    ExperienceComponent const& gain(
        ExperienceComponent& component,
        int value
    )
    {
        component.experienceCurrent += value;

        if ( component.experienceCurrent < 0 )
        {
            component.experienceCurrent = 0;
        }

        if ( component.experienceCurrent < component.levelUpThreshold )
        {
            return component;
        }

        ++component.level;

        component.levelUpThreshold = getNewThreshold( component.level );

        return component;
    }
}
