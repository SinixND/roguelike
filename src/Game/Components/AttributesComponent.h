#ifndef IG20250307140313
#define IG20250307140313

struct HealthComponent;
struct EnergyComponent;

int constexpr ATTRIBUTES{ 2 };

struct AttributesComponent
{
    int vitality{ 10 }; //* Health
    // int Strength{};  //* Damage
    // int Defense{};   //* Damage reduction
    // int Dexterity{}; //* CHD
    int agility{ 10 }; //* Energy regen
    // int Luck{};      //* Evasion (and CHC?)
    // int Awareness{}; //* Vision range
};

namespace AttributesModule
{
    int totalPoints( AttributesComponent const& attributes );
}

#endif

