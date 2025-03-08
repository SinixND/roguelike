#ifndef IG20250306120651
#define IG20250306120651

int constexpr BASE_EXPERIENCE = 10;

struct ExperienceComponent
{
    int level{ 1 };
    int current{ 0 };
    int levelUpThreshold{ BASE_EXPERIENCE };
};

namespace ExperienceModule
{
    int getExpValue( int foeExpLevel, int attackerExpLevel );

    [[nodiscard]]
    ExperienceComponent const& gainExp(
        ExperienceComponent& experience,
        int value
    );
}

#endif
