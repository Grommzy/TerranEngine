#ifndef ATLASUTIL_H
#define ATLASUTIL_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    inline glm::vec4 TileUV(int tileX, int tileY, int tileSizePx, int atlasWidth, int atlasHeight)
    {
        const float u  =  tileX * tileSizePx / static_cast<float>(atlasWidth);
        const float v  =  tileY * tileSizePx / static_cast<float>(atlasHeight);
        const float uSize =  tileSizePx      / static_cast<float>(atlasWidth);
        const float vSize =  tileSizePx      / static_cast<float>(atlasHeight);

        return { u, v, uSize, vSize };
    }
}

#endif // ATLASUTIL_H