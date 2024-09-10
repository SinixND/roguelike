#include "Renderer.h"
#define DEBUG_CHUNKS
#define DEBUG_VISIBILITY

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
    textures_.registerTexture(RenderID::goblin, {70, 35});
}

void Renderer::render(
    RenderID renderID,
    Vector2 const& worldPixel,
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
            worldPixel.x,
            worldPixel.y,
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        // TileData::TILE_CENTER,
        Vector2{0,0},
        0,
        tint);
}

void Renderer::renderToChunk(
    RenderID renderID,
    Vector2 const& worldPixel,
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
            worldPixel.x - chunk.position().worldPixel().x,
            worldPixel.y - chunk.position().worldPixel().y,
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        // TileData::TILE_CENTER,
        Vector2{0,0},
        0,
        tint);
}

void Renderer::renderChunk(Chunk& chunk)
{
    DrawTexturePro(
        chunk.renderTexture().texture,
        Rectangle{
            0,
            0,
            static_cast<float>(chunk.renderTexture().texture.width),
            static_cast<float>(-chunk.renderTexture().texture.height)},
        Rectangle{
            chunk.position().worldPixel().x,
            chunk.position().worldPixel().y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        Vector2{0, 0},
        0,
        WHITE);

#if defined(DEBUG) && defined(DEBUG_CHUNKS)
    DrawRectangleLinesEx(
        Rectangle{
            chunk.position().worldPixel().x,
            chunk.position().worldPixel().y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        3.0f,
        YELLOW);
#endif
}

void Renderer::renderFog(Fog const& fog)
{
    Color tint{};

#if defined(DEBUG) && defined(DEBUG_VISIBILITY)
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
        // Vector2SubtractValue(
            UnitConversion::tileToWorld(fog.tilePosition()),
            // TileData::TILE_SIZE_HALF),
        TileData::TILE_DIMENSIONS,
        tint);
}
void Renderer::deinit()
{
    textures_.unloadAtlas();
}
