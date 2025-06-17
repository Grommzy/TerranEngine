#ifndef TILEGRID_H
#define TILEGRID_H

#include "engine/ecs/Entity.h"

#include <vector>
#include <glm/glm.hpp>

namespace TerranEngine
{
    using TileID = int;

    class TileGrid
    {
    public:
        void Resize(int width, int height);

        [[nodiscard]] int Width() const noexcept { return width; }
        [[nodiscard]] int Height() const noexcept { return height; }

        [[nodiscard]] bool InBounds(int x, int y) const noexcept;

        [[nodiscard]] TileID GetTile(int x, int y) const noexcept;
        void                 SetTile(int x, int y, TileID id);

        [[nodiscard]] Entity GetEntity(int x, int y) const noexcept;
        void                 SetEntity(int x, int y, Entity entity);

        [[nodiscard]] glm::vec2 GridToWorld(glm::ivec2 gridPosition, int tilePx = 16) const noexcept { return { static_cast<float>(gridPosition.x * tilePx), static_cast<float>(gridPosition.y * tilePx) }; }
    
    private:
        int width {0};
        int height {0};

        std::vector<TileID> tileGrid;   // Size = Width * Height
        std::vector<Entity> entityGrid; // Size = Width * Height
    };
}

#endif // TILEGRID_H