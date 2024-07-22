#include "Renderer.h"
#include "RenderID.h"
#include "TextureData.h"
#include "Textures.h"
#include "TileData.h"
#include <raylib.h>

void Renderer::init()
{
    // Load texture atlas
    textures_.loadAtlas(TextureData::TEXTURE_ATLAS_FILE);

    // Register textures
    textures_.registerTexture(RenderID::invisible, {0, 0});
    textures_.registerTexture(RenderID::cursor, {35, 0});
    textures_.registerTexture(RenderID::hero, {70, 0});
    textures_.registerTexture(RenderID::wall, {105, 0});
    textures_.registerTexture(RenderID::floor, {0, 35});
    textures_.registerTexture(RenderID::reachable, {35, 35});
    textures_.registerTexture(RenderID::path, {70, 35});
    textures_.registerTexture(RenderID::attackable, {105, 35});
    textures_.registerTexture(RenderID::supportable, {0, 70});
    textures_.registerTexture(RenderID::nextLevel, {35, 70});
}

void Renderer::render(RenderID renderID, Vector2 const& renderPosition, Color const& tint)
{
    // Use 0.5f pixel offset to avoid texture bleeding
    DrawTexturePro(
        textures_.getTextureAtlas(),
        Rectangle{
            textures_.getTexturePosition(renderID).x + 0.5f,
            textures_.getTexturePosition(renderID).y + 0.5f,
            TextureData::TEXTURE_SIZE - (2 * 0.5f),
            TextureData::TEXTURE_SIZE - (2 * 0.5f)},
        Rectangle{
            renderPosition.x,
            renderPosition.y,
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        TileData::TILE_CENTER,
        0,
        tint);
}

void Renderer::deinit()
{
    textures_.unloadAtlas();
}
