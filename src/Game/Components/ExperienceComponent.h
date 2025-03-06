#ifndef IG20250306120651
#define IG20250306120651

int constexpr baseExperience = 10;

struct ExperienceComponent
{
    int expLevel{ 1 };
    int expCurrent{ 0 };
    int levelUpThreshold{ baseExperience };
};

namespace ExperienceModule
{
    [[nodiscard]]
    ExperienceComponent const& levelUp( ExperienceComponent& component );

    [[nodiscard]]
    ExperienceComponent const& levelUp(
        ExperienceComponent& component,
        int expLevel
    );

    [[nodiscard]]
    ExperienceComponent const& gainExp(
        ExperienceComponent& component,
        int value
    );
}

#endif
