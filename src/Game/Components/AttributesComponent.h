#ifndef IG20250307140313
#define IG20250307140313

struct HealthComponent;
struct EnergyComponent;

struct AttributesComponent
{
    int vitality{}; //* Health
    // int Strength{};  //* Damage
    // int Defense{};   //* Damage reduction
    // int Dexterity{}; //* CHD
    int agility{}; //* Energy regen
    // int Luck{};      //* Evasion (and CHC?)
    // int Awareness{}; //* Vision range
};

#endif

