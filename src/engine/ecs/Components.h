#ifndef TERRANENGINE_COMPONENTS_H
#define TERRANENGINE_COMPONENTS_H

#include "engine/ecs/Behaviour.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace TerranEngine
{
    struct Transform2D
    {
        glm::vec2 position {0.0f, 0.0f};
        glm::vec2 scale    {1.0f, 1.0f};
        float     rotation {0};
    };

    struct Sprite
    {
        GLuint texture {0};
        glm::vec2 size {0.0f, 0.0f};
    };
}

#endif // TERRANENGINE_COMPONENTS_H