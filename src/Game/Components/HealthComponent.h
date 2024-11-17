#ifndef IG20241028234910
#define IG20241028234910

struct HealthComponent
{
    int currentHealth{1};
    int maxHealth{currentHealth};
    int regenRate{0};
};

// Returns true if health is <= 0
// Reduces health to 0 with no argument
bool damage(
    HealthComponent* healthComponent,
    int health = 0);

// Fully heals with no argument
void heal(
    HealthComponent* healthComponent,
    int health = 0);

void regenerate(HealthComponent* healthComponent);

#endif
