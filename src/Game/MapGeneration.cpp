#include "MapGeneration.h"

#include "CPosition.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "RuntimeDatabase.h"
#include "TIsSolid.h"
#include "TIsTile.h"
#include "TRenderedAsMap.h"
#include <raylib.h>
#include <raymath.h>

void addRoom(snd::SparseSet<Vector2, snd::EntityId>* tileMap, const Area& room, snd::ECS* ecs)
{
    Vector2 bottomRightCorner{Vector2Add(room.position, room.dimension)};

    for (auto x{room.position.x}; x < bottomRightCorner.x; ++x)
    {
        for (auto y{room.position.y}; y < bottomRightCorner.y; ++y)
        {
            Vector2 position{x, y};

            snd::EntityId newTileEntity{createNewMapTileEntitiy(position, ecs)};

            tileMap->insert(position, newTileEntity);

            // Assign wall components
            if ((x == room.position.x) || (x == (bottomRightCorner.y - 1)) || (y == room.position.y) || (y == (bottomRightCorner.y - 1)))
            {
                ecs->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(WALL_TEXTURE));
                ecs->assignComponent<TIsSolid>(newTileEntity);
                continue;
            }

            // Assign floor components
            ecs->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(FLOOR_TEXTURE));
        }
    }
};

void setTiles(snd::SparseSet<Vector2, snd::EntityId>* tileMap, const Area& area, TileType tileType, snd::ECS* ecs)
{
    Vector2 bottomRightCorner{Vector2Add(area.position, area.dimension)};

    for (auto x{area.position.x}; x < bottomRightCorner.x; ++x)
    {
        for (auto y{area.position.y}; y < bottomRightCorner.y; ++y)
        {
            Vector2 position{x, y};

            snd::EntityId tileEntity{};

            // Create new tile entity if it does not exist yet
            if (tileMap->test(position))
            {
                tileEntity = *tileMap->get(position);
            }
            else
            {
                tileEntity = createNewMapTileEntitiy(position, ecs);
            }

            // Assign wall components
            switch (tileType)
            {
            case WALL_TILE:
                ecs->assignComponent<CTexture>(tileEntity, dtb::Textures::get(WALL_TEXTURE));
                ecs->assignComponent<TIsSolid>(tileEntity);
                break;

            case FLOOR_TILE:
            default:
                ecs->assignComponent<CTexture>(tileEntity, dtb::Textures::get(FLOOR_TEXTURE));
                break;
            }
        };
    }
};

snd::EntityId createNewMapTileEntitiy(Vector2 position, snd::ECS* ecs)
{
    // Make new tile entity
    snd::EntityId newTileEntity{ecs->createEntity()};

    // Assign tile components
    ecs->assignComponent<CPosition>(newTileEntity, position.x, position.y);
    ecs->assignComponent<CTransform>(newTileEntity);
    ecs->assignComponent<TRenderedAsMap>(newTileEntity);
    ecs->assignComponent<TIsTile>(newTileEntity);

    return newTileEntity;
}

void setStartRoom(snd::SparseSet<Vector2, snd::EntityId>* tileMap, snd::ECS* ecs)
{
    addRoom(tileMap, Area{{-7, -7}, {15, 15}}, ecs);
    setTiles(tileMap, Area{{-1, -1}, {3, 1}}, WALL_TILE, ecs);
}