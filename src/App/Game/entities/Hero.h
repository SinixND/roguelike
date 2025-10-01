#ifndef IG20240531143458
#define IG20240531143458

#include "ActionId.h"
#include "AttackComponent.h"
#include "AttributesComponent.h"
#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "ExperienceComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MoveComponent.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <memory>

struct Hero
{
    //* Persistent components
    AttributesComponent attributes{};
    EnergyComponent energy{};
    HealthComponent health{
        HeroData::HEALTH_BASE,
        HeroData::HEALTH_REGEN_BASE,
    };
    ExperienceComponent experience{};
    DamageComponent damage{ HeroData::DAMAGE_BASE };
    Vector2 position{};
    RenderId renderId{ RenderId::HERO };
    int visionRange{ HeroData::VISION_RANGE };

    //* Optional components / actions
    std::shared_ptr<ActionId> action{};
    std::shared_ptr<AttackComponent> attack{};
    std::shared_ptr<MoveComponent> move{};
    std::vector<Vector2I> path{};
    bool awaitsInput{};
};

#endif
