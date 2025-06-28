#include "engine/ecs/world/HierarchySystem.h"

#include "engine/ecs/components/Relationship.h"
#include "engine/ecs/components/Transform2D.h"
#include "engine/ecs/world/World.h"

namespace TerranEngine
{
    void HierarchySystem::Update(World& world, float)
    {
        world.ForEach<Relationship, Transform2D>([&](Entity, Relationship& relationship, Transform2D& childTransform)
        {
            if (!relationship.parent) { return; }

            Transform2D* parentTransform = world.GetComponent<Transform2D>(relationship.parent);
            if (!parentTransform) { return; } // Check for `nullptr` return.

            childTransform.position = parentTransform->position + relationship.transformOffset;

            if (relationship.inheritScale)    { childTransform.scale = parentTransform->scale; }
            if (relationship.inheritRotation) { childTransform.rotation = parentTransform->rotation; }
        });
    }
}