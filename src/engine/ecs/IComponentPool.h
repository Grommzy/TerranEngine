#ifndef ICOMPONENTPOOL_H
#define ICOMPONENTPOOL_H

#include "engine/ecs/Entity.h"

namespace TerranEngine
{
    // Type-erased base such that Registry can call Remove() on any pool.
    struct IComponentPool
    {
        virtual void Remove(Entity entity) = 0;
        virtual ~IComponentPool()          = default;
    };
}

#endif // ICOMPONENTPOOL_H