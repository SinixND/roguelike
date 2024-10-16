#include "RenderSystem.h"
#include <string>
//* #define DEBUG_CHUNKS
#define DEBUG_VISIBILITY

#include "Chunk.h"
#include "ChunkData.h"
#include "RenderID.h"
#include "TextureData.h"
#include "Textures.h"
#include "TileData.h"
#include "UnitConversion.h"
#include "VisibilitySystem.h"
#include <raylib.h>
#include <raymath.h>

std::string RenderSystem::textureAtlasFileName() const
{
    return TextureData::themes[theme_] + ".png";
}

void RenderSystem::init()
{
    //* Load texture atlas
    textures_.loadAtlas(textureAtlasFileName());

    //* Register textures
    textures_.registerTexture(RenderID::none, {0, 0});
    textures_.registerTexture(RenderID::cursor, {35, 0});
    textures_.registerTexture(RenderID::hero, {70, 0});
    textures_.registerTexture(RenderID::wall, {105, 0});
    textures_.registerTexture(RenderID::floor, {0, 35});
    textures_.registerTexture(RenderID::descend, {35, 35});
    textures_.registerTexture(RenderID::ascend, {70, 35});
    textures_.registerTexture(RenderID::goblin, {105, 35});
}

void RenderSystem::render(
    RenderID renderID,
    Vector2 const& worldPixel,
    Color const& tint) const
{
    //* Use 0.5f pixel offset to avoid texture bleeding
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
        //* TileData::TILE_CENTER,
        Vector2{0, 0},
        0,
        tint);
}

void RenderSystem::renderToChunk(
    RenderID renderID,
    Vector2 const& worldPixel,
    Chunk& chunk,
    Color const& tint) const
{
    //* Use 0.5f pixel offset to avoid texture bleeding
    DrawTexturePro(
        textures_.getTextureAtlas(),
        Rectangle{
            textures_.getTexturePosition(renderID).x + 0.5f,
            textures_.getTexturePosition(renderID).y + 0.5f,
            TextureData::TEXTURE_SIZE - (2 * 0.5f),
            TextureData::TEXTURE_SIZE - (2 * 0.5f)},
        Rectangle{
            worldPixel.x - chunk.position.worldPixel().x,
            worldPixel.y - chunk.position.worldPixel().y,
            TileData::TILE_SIZE,
            TileData::TILE_SIZE},
        //* TileData::TILE_CENTER,
        Vector2{0, 0},
        0,
        tint);
}

void RenderSystem::renderChunk(Chunk const& chunk) const
{
    DrawTexturePro(
        chunk.renderTexture.texture,
        Rectangle{
            0,
            0,
            static_cast<float>(chunk.renderTexture.texture.width),
            static_cast<float>(-chunk.renderTexture.texture.height)},
        Rectangle{
            chunk.position.worldPixel().x,
            chunk.position.worldPixel().y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        Vector2{0, 0},
        0,
        WHITE);

#if defined(DEBUG) && defined(DEBUG_CHUNKS)
    DrawRectangleLinesEx(
        Rectangle{
            chunk.position.worldPixel().x,
            chunk.position.worldPixel().y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        3.0f,
        YELLOW);
#endif
}

void RenderSystem::renderFog(Fog const& fog) const
{
    Color tint{};

#if defined(DEBUG) && defined(DEBUG_VISIBILITY)
    if (fog.isFogOpaque)
    {
        tint = ColorAlpha(RED, 0.5f);
    }

    else
    {
        tint = ColorAlpha(BLUE, 0.5f);
    }
#else
    if (fog.isFogOpaque)
    {
        tint = BLACK;
    }

    else
    {
        tint = ColorAlpha(BLACK, 0.5f);
    }
#endif

    DrawRectangleV(
        //* Vector2SubtractValue(
        UnitConversion::tileToWorld(fog.tilePosition),
        //* TileData::TILE_SIZE_HALF),
        TileData::TILE_DIMENSIONS,
        tint);
}

void RenderSystem::cycleThemes()
{
    theme_ = (theme_ < TextureData::themes.size() - 1) ? ++theme_ : 0;
}

void RenderSystem::deinit()
{
    textures_.unloadAtlas();
}
