#ifndef IG20250306120651
#define IG20250306120651

int constexpr baseExperience = 10;

struct ExperienceComponent
{
    int level{ 1 };
    int experienceCurrent{ 0 };
    int levelUpThreshold{ baseExperience };
};

namespace ExperienceModule
{
    [[nodiscard]]
    ExperienceComponent const& gain(
        ExperienceComponent& component,
        int value
    );
}

#endif
