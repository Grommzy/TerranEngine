#ifndef TERRANENGINE_RELATIONSHIP_H
#define TERRANENGINE_RELATIONSHIP_H

#include "engine/ecs/Entity.h"
#include <glm/glm.hpp>

namespace TerranEngine
{
    struct Relationship
    {
        Entity parent             {Entity(0)};
        glm::vec2 transformOffset {0.0f, 0.0f};
        bool inheritScale         {true};
        bool inheritRotation      {false};
    };
}

#endif // TERRANENGINE_RELATIONSHIP_H