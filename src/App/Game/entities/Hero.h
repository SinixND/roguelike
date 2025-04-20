#ifndef IG20240531143458
#define IG20240531143458

#include "AttributesComponent.h"
#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "ExperienceComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"

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

    //* Optional components
    //* Old
    //* TODO: CHANGE/REMOVE
    TransformComponent transform{};
    MovementComponent movement{};
    //* New
    bool isReady{};
};

struct Map;
struct Cursor;
struct GameCamera;
enum class InputId;

namespace HeroModule
{
    [[nodiscard]]
    Hero const& executeAction(
        Hero& hero,
        Map& mapIO,
        Cursor const& cursor,
        GameCamera const& gameCamera,
        InputId currentInput
    );
};

#endif
