#include "Renderer.h"

#include "Chunk.h"
#include "ChunkData.h"
#include "RenderID.h"
#include "TextureData.h"
#include "Textures.h"
#include "TileData.h"
#include "UnitConversion.h"
#include "Visibility.h"
#include <raylib.h>
#include <raymath.h>

void Renderer::init()
{
    // Load texture atlas
    textures_.loadAtlas(TextureData::TEXTURE_ATLAS_FILE);

    // Register textures
    textures_.registerTexture(RenderID::none, {0, 0});
    textures_.registerTexture(RenderID::cursor, {35, 0});
    textures_.registerTexture(RenderID::hero, {70, 0});
    textures_.registerTexture(RenderID::wall, {105, 0});
    textures_.registerTexture(RenderID::floor, {0, 35});
    textures_.registerTexture(RenderID::nextLevel, {35, 35});
}

void Renderer::render(
    RenderID renderID,
    Vector2 const& worldPosition,
    Color const& tint)
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
            worldPosition.x,
            worldPosition.y,
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        TileData::TILE_CENTER,
        0,
        tint);
}

void Renderer::renderChunk(Chunk& chunk)
{
    RenderTexture& renderTexture{chunk.renderTexture()};
    Vector2 worldPosition{chunk.position().worldPosition()};

    DrawTexturePro(
        renderTexture.texture,
        Rectangle{
            0,
            0,
            static_cast<float>(renderTexture.texture.width),
            static_cast<float>(-renderTexture.texture.height)},
        Rectangle{
            worldPosition.x,
            worldPosition.y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        ChunkData::CHUNK_CENTER,
        0,
        WHITE);
}

void Renderer::renderToChunk(
    RenderID renderID,
    Vector2 const& worldPosition,
    Chunk& chunk,
    Color const& tint)
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
            worldPosition.x - chunk.position().worldPosition().x + (ChunkData::CHUNK_SIZE_F / 2),
            worldPosition.y - chunk.position().worldPosition().y + (ChunkData::CHUNK_SIZE_F / 2),
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        TileData::TILE_CENTER,
        0,
        tint);
}

void Renderer::renderFog(Fog const& fog)
{
    Color tint{};

#if defined(DEBUG) && defined(DEBUG_FOG)
    if (fog.isFogOpaque())
    {
        tint = ColorAlpha(RED, 0.5f);
    }
    else
    {
        tint = ColorAlpha(BLUE, 0.5f);
    }
#else
    if (fog.isFogOpaque())
    {
        tint = BLACK;
    }
    else
    {
        tint = ColorAlpha(BLACK, 0.5f);
    }
#endif

    DrawRectangleV(
        Vector2SubtractValue(
            UnitConversion::tileToWorld(fog.tilePosition()),
            TileData::TILE_SIZE_HALF),
        TileData::TILE_DIMENSIONS,
        tint);
}
void Renderer::deinit()
{
    textures_.unloadAtlas();
}
