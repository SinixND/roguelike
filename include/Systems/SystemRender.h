#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "CONSTANTS.h"
#include "ComponentManager.h"
#include "ComponentOrientation.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "EntityId.h"
#include "System.h"
#include <iostream>
#include <raylib.h>

namespace snd
{
    class RenderSystem : public System<TextureComponent, PositionComponent, DirectionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get component pointers
            const auto& texture{
                componentManager_.retrieveFrom<TextureComponent>(entityId)->texture_};

            const auto& position{
                componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            const auto& direction{
                componentManager_.retrieveFrom<DirectionComponent>(entityId)->directionDeg_};

            // Action
            const auto& CONSTANTS{CONSTANTS::getInstance()};

            DrawTexturePro(
                *texture,
                Rectangle{
                    0,
                    0,
                    float(texture->width),
                    float(texture->height)},
                Rectangle{
                    position.x,
                    position.y,
                    CONSTANTS->getTileSize().x,
                    CONSTANTS->getTileSize().y},
                Vector2{
                    0,
                    0},
                direction,
                WHITE);
        }

        RenderSystem(ComponentManager& componentManager)
            : System<TextureComponent, PositionComponent, DirectionComponent>(componentManager)
        {
        }
    };
}

#endif