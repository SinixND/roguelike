#include "RestComponent.h"

namespace RestModule
{
    RestComponent const& rest(
        RestComponent& resting,
        size_t turn
    )
    {
        if ( resting.lastRest == ( turn - 1 ) )
        {
            ++resting.consecutiveRests;
        }

        resting.consecutiveRests = 1;

        return resting;
    }
}

