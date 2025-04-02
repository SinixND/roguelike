#ifndef IG20250307140313
#define IG20250307140313

struct HealthComponent;
struct EnergyComponent;

int constexpr ATTRIBUTES{ 4 };
int constexpr BASE_POINTS{ 10 };

struct AttributesComponent
{
    int vitality{ BASE_POINTS }; //* Health
    int strength{ BASE_POINTS }; //* Damage
    int defense{ BASE_POINTS };  //* Damage reduction
    // int Dexterity{BASE_POINTS}; //* CHD
    int agility{ BASE_POINTS }; //* Energy regen
    // int Luck{BASE_POINTS};      //* Evasion (and CHC?)
    // int Awareness{BASE_POINTS}; //* Vision range
};

namespace AttributesModule
{
    int totalPoints( AttributesComponent const& attributes );
}

#endif

