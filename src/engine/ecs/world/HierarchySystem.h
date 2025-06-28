#ifndef TERRANENGINE_HIERARCHYSYSTEM_H
#define TERRANENGINE_HIERARCHYSYSTEM_H

#include "engine/ecs/System.h"

namespace TerranEngine
{
    class HierarchySystem final : public System
    {
    public:
        HierarchySystem()  = default;
        ~HierarchySystem() = default;

        void Update(World& world, float deltaTime);
    };
}

#endif // TERRANENGINE_HIERARCHYSYSTEM_H