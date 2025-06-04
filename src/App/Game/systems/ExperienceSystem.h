#ifndef IG20250309184551
#define IG20250309184551

struct ExperienceComponent;
struct HealthComponent;

namespace ExperienceSystem
{
    [[nodiscard]]
    ExperienceComponent const& levelUp( ExperienceComponent& experience );

    [[nodiscard]]
    ExperienceComponent const& levelUpTo(
        ExperienceComponent& experience,
        int expLevel
    );

    [[nodiscard]]
    HealthComponent const& raiseHealth(
        HealthComponent& healthIO,
        int level
    );

    void updateStats(
        HealthComponent& healthIO
    );
}

#endif

