#include "engine/level/TileGrid.h"
#include <algorithm>

namespace TerranEngine
{
    void TileGrid::Resize(int width, int height)
    {
        int oldWidth  = this->width;
        int oldHeight = this->height;
        if (width == oldWidth && height == oldHeight)
            return;

        // allocate new buffers
        std::vector<TileID> newTiles(width * height, -1);
        std::vector<Entity> newEntities(width * height, NullEntity);

        // copy existing data into new buffers
        int copyWidth  = std::min(oldWidth, width);
        int copyHeight = std::min(oldHeight, height);
        for (int y = 0; y < copyHeight; ++y)
        {
            for (int x = 0; x < copyWidth; ++x)
            {
                newTiles[y * width + x]     = tileGrid[y * oldWidth + x];
                newEntities[y * width + x]  = entityGrid[y * oldWidth + x];
            }
        }

        // replace old grids
        tileGrid.swap(newTiles);
        entityGrid.swap(newEntities);

        // update dimensions
        this->width  = width;
        this->height = height;
    }

    bool TileGrid::InBounds(int x, int y) const noexcept
    {
        return (unsigned)x < (unsigned)width && (unsigned)y < (unsigned)height;
    }

    TileID TileGrid::GetTile(int x, int y) const noexcept
    {
        assert(InBounds(x, y));
        return tileGrid[y * width + x];
    }

    void TileGrid::SetTile(int x, int y, TileID id)
    {
        assert(InBounds(x, y));
        tileGrid[y * width + x] = id;
    }

    Entity TileGrid::GetEntity(int x, int y) const noexcept
    {
        assert(InBounds(x, y));
        return entityGrid[y * width + x];
    }

    void TileGrid::SetEntity(int x, int y, Entity entity)
    {
        assert(InBounds(x, y));
        entityGrid[y * width + x] = entity;
    }
}