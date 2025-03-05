#ifndef IG20250304150507
#define IG20250304150507

#include <cstddef>

struct RestComponent
{
    size_t lastRest{ 0 };
    int consecutiveRests{ 1 };

    RestComponent() = default;
    RestComponent( size_t turn )
        : lastRest( turn )
    {
    }
};

namespace RestModule
{
    [[nodiscard]]
    RestComponent const& rest(
        RestComponent& resting,
        size_t turn
    );
}

#endif
