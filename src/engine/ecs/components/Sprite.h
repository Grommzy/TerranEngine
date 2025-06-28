#ifndef TERRANENGINE_SPRITE_H
#define TERRANENGINE_SPRITE_H

#include "engine/gfx/Texture.h"

#include <glm/glm.hpp>

namespace TerranEngine
{
    struct Sprite
    {
        const Texture* texture {nullptr};                // Texture atlas to sample from.
        glm::vec2 size         {16.0f, 16.0f};           // Tile size in pixels (width x height)
        uint32_t atlasIndex    {0};                      // Index of the tile inside the atlas (row-major)
        glm::vec4 tint         {1.0f, 1.0f, 1.0f, 1.0f}; // Per-sprite RGBA tint (default opaque white)
        glm::vec2 anchor       {0.5f, 0.5f};             // Alignment of sprite to world-position. Range 0-1
        glm::vec2 origin       {0.5f, 0.5f};             // Pivot point for rotation and scaling. Range 0-1
        int32_t zLevel         {0};                      // Z-layer of the sprite. 0 = mid-layer.
    };
}

#endif // TERRANENGINE_SPRITE_H