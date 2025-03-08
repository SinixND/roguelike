#ifndef IG20250306120651
#define IG20250306120651

int constexpr BASE_EXPERIENCE = 10;

struct ExperienceComponent
{
    int expLevel{ 1 };
    int expCurrent{ 0 };
    int levelUpThreshold{ BASE_EXPERIENCE };
};

namespace ExperienceModule
{
    [[nodiscard]]
    ExperienceComponent const& levelUp( ExperienceComponent& experience );

    [[nodiscard]]
    ExperienceComponent const& levelUp(
        ExperienceComponent& experience,
        int expLevel
    );

    [[nodiscard]]
    ExperienceComponent const& gainExp(
        ExperienceComponent& experience,
        int value
    );
}

#endif
