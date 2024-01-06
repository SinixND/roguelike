#ifndef SYSTEMCONTROL_H_20240106170456
#define SYSTEMCONTROL_H_20240106170456

#include "ComponentManager.h"
#include "ComponentPosition.h"
#include "EntityId.h"
#include "FlagControl.h"
#include "System.h"
#include "Utility.h"
#include <raylib.h>
namespace snd
{
    class ControlSystem : public System<ControlFlag, PositionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            // Action
            position = Utility::coordinateToPosition(GetMousePosition());
        }

        ControlSystem(ComponentManager& componentManager)
            : System<ControlFlag, PositionComponent>(componentManager)
        {
        }
    };
}

#endif