#ifndef TERRANENGINE_TRANSFORM2D_H
#define TERRANENGINE_TRANSFORM2D_H

#include <glm/glm.hpp>

namespace TerranEngine
{
    struct Transform2D
    {
        glm::vec2 position {0.0f, 0.0f};
        glm::vec2 scale    {1.0f, 1.0f};
        float     rotation {0};
    };
}

#endif // TERRANENGINE_TRANSFORM2D_H