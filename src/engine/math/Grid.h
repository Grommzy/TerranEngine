#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    // Convert integer grid cooridnates to world-pixel coordinates (top-left of tile)
    inline glm::vec2 GridToWorld(const glm::ivec2& gridPosition, int tilePx = 16) noexcept
    {
        return glm::vec2(gridPosition) * static_cast<float>(tilePx);
    }

    // Convert world-pixel coordinates to integer grid coordinates (floors result)
    inline glm::ivec2 WorldToGrid(const glm::vec2& worldPosition, int tilePx = 16) noexcept
    {
        return glm::floor(worldPosition / static_cast<float>(tilePx));
    }
}

#endif // GRID_H