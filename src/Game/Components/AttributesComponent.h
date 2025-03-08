#ifndef IG20250307140313
#define IG20250307140313

struct AttributesComponent
{
    int Vitality{};  //* Health
    int Strength{};  //* Damage
    int Dexterity{}; //* Energy cost reduction or Actions per turn (and CHD?)
    int Defense{};   //* Damage reduction
    int Agility{};   //* Energy regen (Evasion and CHC?)
    int Awareness{}; //* Vision range
    int Weight{};    //* Energy regen
};

#endif

