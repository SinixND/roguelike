#include "RenderSystem.h"
#include "Data/TextureData.h"
#include "RenderData.h"

//* #define DEBUG_CHUNKS
#define DEBUG_VISIBILITY

#include "Chunk.h"
#include "ChunkData.h"
#include "Convert.h"
#include "RenderId.h"
#include "TextureData.h"
#include "Textures.h"
#include "TileData.h"
#include "VisibilitySystem.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <string>

std::string textureAtlasFileName(size_t theme)
{
    return TextureData::themes[theme] + ".png";
}

void RenderSystem::loadRenderData(RenderData& renderData)
{
    //* Load texture atlas
    renderData.textures.loadAtlas(
        textureAtlasFileName(
            renderData.theme));

    //* Register textures
    renderData.textures.registerTexture(RenderId::NONE, {0, 0});
    renderData.textures.registerTexture(RenderId::CURSOR, {35, 0});
    renderData.textures.registerTexture(RenderId::HERO, {70, 0});
    renderData.textures.registerTexture(RenderId::WALL, {105, 0});
    renderData.textures.registerTexture(RenderId::FLOOR, {0, 35});
    renderData.textures.registerTexture(RenderId::DESCEND, {35, 35});
    renderData.textures.registerTexture(RenderId::ASCEND, {70, 35});
    renderData.textures.registerTexture(RenderId::GOBLIN, {105, 35});
}

void RenderSystem::render(
    Textures const& textures,
    RenderId renderId,
    Vector2 const& worldPixel,
    Color const& tint)
{
    //* Use 0.5f pixel offset to avoid texture bleeding
    DrawTexturePro(
        textures.textureAtlas(),
        Rectangle{
            textures.getTexturePosition(renderId).x + 0.5f,
            textures.getTexturePosition(renderId).y + 0.5f,
            TextureData::textureSize - (2 * 0.5f),
            TextureData::textureSize - (2 * 0.5f)},
        Rectangle{
            worldPixel.x,
            worldPixel.y,
            TileData::tileSize,
            TileData::tileSize},
        //* TileData::TILE_CENTER,
        Vector2{
            0,
            0},
        0,
        tint);
}

void RenderSystem::renderToChunk(
    Textures const& textures,
    RenderId renderId,
    Vector2 const& worldPixel,
    Chunk& chunk,
    Color const& tint)
{
    //* Use 0.5f pixel offset to avoid texture bleeding
    DrawTexturePro(
        textures.textureAtlas(),
        Rectangle{
            textures.getTexturePosition(renderId).x + 0.5f,
            textures.getTexturePosition(renderId).y + 0.5f,
            TextureData::textureSize - (2 * 0.5f),
            TextureData::textureSize - (2 * 0.5f)},
        Rectangle{
            worldPixel.x - chunk.position.worldPixel.x,
            worldPixel.y - chunk.position.worldPixel.y,
            TileData::tileSize,
            TileData::tileSize},
        //* TileData::TILE_CENTER,
        Vector2{0, 0},
        0,
        tint);
}

void RenderSystem::renderChunk(Chunk const& chunk)
{
    DrawTexturePro(
        chunk.renderTexture.texture,
        Rectangle{
            0,
            0,
            static_cast<float>(chunk.renderTexture.texture.width),
            static_cast<float>(-chunk.renderTexture.texture.height)},
        Rectangle{
            chunk.position.worldPixel.x,
            chunk.position.worldPixel.y,
            ChunkData::chunkSize_f,
            ChunkData::chunkSize_f},
        Vector2{0, 0},
        0,
        WHITE);

#if defined(DEBUG) && defined(DEBUG_CHUNKS)
    DrawRectangleLinesEx(
        Rectangle{
            chunk.position.worldPixel.x,
            chunk.position.worldPixel.y,
            ChunkData::CHUNK_SIZE_F,
            ChunkData::CHUNK_SIZE_F},
        3.0f,
        YELLOW);
#endif
}

void RenderSystem::renderFog(Fog const& fog)
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
        Convert::tileToWorld(fog.tilePosition),
        //* TileData::TILE_SIZE_HALF),
        TileData::tileDimensions,
        tint);
}

void RenderSystem::cycleThemes(size_t& theme)
{
    theme = (theme < TextureData::themes.size() - 1) ? ++theme : 0;
}

void RenderSystem::deinit(Textures& textures)
{
    textures.unloadAtlas();
}
