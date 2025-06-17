#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "engine/gfx/Texture.h"
#include <glm/glm.hpp>
#include <vector>

namespace TerranEngine
{
    struct Transform2D
    {
        glm::vec2 position {0.0f, 0.0f};
        glm::vec2 scale    {1.0f, 1.0f};
        float     rotation {0};
    };

    struct SpriteComponent
    {
        const Texture* texture {nullptr};          // Pointer to texture/atlas
        int atlasIndex   {0};                      // Tile/Frame index
        glm::ivec2 size  {16, 16};                 // XscalePx, YscalePx
        glm::vec2 anchor {0.0f, 0.0f};             // '0 - 1' range, '0, 0' = top-left. Effects translations.
        glm::vec2 origin {0.5f, 0.5f};             // '0 - 1' range, '0, 0' = top-left. Effects transformations.
        glm::vec4 tint   {1.0f, 1.0f, 1.0f, 1.0f}; // '0 - 1' range, RGBA
        int zLevel       {0};                      // '-100 - 200' range, Z
    };

    struct AnimationComponent
    {
        std::vector<int> frames;           // Texture Atlas indices
        float            frameTime {0.2f}; // Seconds per frame
        float            timer     {0.0f}; // Keeps track of frame-times
        size_t           cursor    {0};    // Pointer to animation-frame
    };
}

#endif // COMPONENTS_H