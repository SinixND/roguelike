#ifndef IG20250309184551
#define IG20250309184551

struct ExperienceComponent;

namespace LevelUpSystem
{
    [[nodiscard]]
    ExperienceComponent const& levelUp( ExperienceComponent& experience );

    [[nodiscard]]
    ExperienceComponent const& levelUpTo(
        ExperienceComponent& experience,
        int expLevelDelta
    );
}

#endif

