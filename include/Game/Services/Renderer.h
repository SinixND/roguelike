#ifndef RENDERER_H_20240209181841
#define RENDERER_H_20240209181841

#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "Utility.h"
#include <raylib.h>
#include <raylibEx.h>
#include <raymath.h>

class Renderer
{
public:
    void render(RenderId renderId, int x, int y)
    {
        Texture2D* texture{dtb::Textures::get(renderId)};

        Vector2 tileSize{dtb::Constants::getTileDimensions()};
        Vector2 tileCenter{Vector2Scale(tileSize, 0.5f)};

        Vector2 pixelCoordinates{
            Vector2Subtract(
                getTileToScreen(
                    {x, y}),
                tileSize)};

        DrawTexturePro(
            *texture,
            Rectangle{
                0,
                0,
                float(texture->width),
                float(texture->height)},
            Rectangle{
                pixelCoordinates.x,
                pixelCoordinates.y,
                tileSize.x,
                tileSize.y},
            tileCenter,
            0,
            WHITE);
    };
};

#endif