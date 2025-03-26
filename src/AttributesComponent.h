#ifndef IG20250307140313
#define IG20250307140313

struct HealthComponent;
struct EnergyComponent;

int constexpr ATTRIBUTES{ 4 };

struct AttributesComponent
{
    int vitality{ 10 }; //* Health
    int strength{ 10 }; //* Damage
    int defense{ 10 };  //* Damage reduction
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

