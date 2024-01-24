#include "MapGeneration.h"

#include "ECS.h"

void addRoom(snd::ContiguousMap<Vector2, snd::EntityId>* tileMap, const Area& room, snd::ECS* ecs)
{
    for (auto x{room.position.x}; x <= room.dimension.x; ++x)
    {
        for (auto y{room.position.y}; y <= room.dimension.y; ++y)
        {
            Vector2 position{x, y};

            snd::EntityId newTileEntity{createNewMapTileEntitiy(position)};

            tileMap->insert(position, newTileEntity);

            // Assign wall components
            if ((x == room.position.x) || (x == (room.dimension.y)) || (y == room.position.y) || (y == (room.dimension.y)))
            {
                ecs->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(WALL_TEXTURE));
                ecs->assignComponent<TIsSolid>(newTileEntity);
                continue;
            }

            // Assign floor components
            ecs_->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(FLOOR_TEXTURE));
        }
    }
};

void setTiles(snd::ContiguousMap<Vector2, snd::EntityId>* tileMap, const Area& area, TileType tileType)
{
    for (auto x{area.position.x}; x <= area.dimension.x; ++x)
    {
        for (auto y{area.position.y}; y <= area.dimension.y; ++y)
        {
            Vector2 position{x, y};

            snd::EntityId newTileEntity{createNewMapTileEntitiy(position)};

            tileMap->insert(position, newTileEntity);

            // Assign wall components
            switch (tileType)
            {
            case WALL_TILE:
                ecs_->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(WALL_TEXTURE));
                ecs_->assignComponent<TIsSolid>(newTileEntity);
                break;

            case FLOOR_TILE:
            default:
                ecs_->assignComponent<CTexture>(newTileEntity, dtb::Textures::get(FLOOR_TEXTURE));
                break;
            }
        };
    }
};

snd::EntityId createNewMapTileEntitiy(Vector2 position)
{
    // Make new tile entity
    snd::EntityId newTileEntity{ecs_->createEntity()};

    // Assign tile components
    ecs_->assignComponent<CPosition>(newTileEntity, position.x, position.y);
    ecs_->assignComponent<CTransform>(newTileEntity);
    ecs_->assignComponent<TRenderedAsMap>(newTileEntity);

    return newTileEntity;
}