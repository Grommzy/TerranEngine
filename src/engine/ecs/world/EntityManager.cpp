#include "engine/ecs/world/EntityManager.h"

namespace TerranEngine
{
    Entity EntityManager::CreateEntity()
    {
        uint32_t index;

        // If there is a free Entity index, take on the index and effectively "pop" the front of the free-queue.
        if (freeHead != Invalid)
        {
            index = freeHead;
            freeHead = slots[index].nextFree;
        }
        else
        {
            index = static_cast<uint32_t>(slots.size());
            slots.emplace_back();
        }

        Slot& slot = slots[index];
        slot.alive = true;
        const uint32_t generation = slot.generation;

        return Entity { Entity::CreateEntity(index, generation) };
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        if (!IsAlive(entity)) return;

        const uint32_t index = entity.Index();
        Slot& slot = slots[index];

        slot.alive = false;
        slot.generation = (slot.generation + 1u) & GenerationMask;
        slot.nextFree = freeHead;
        freeHead = index;
    }

    bool EntityManager::IsAlive(Entity entity) const noexcept
    {
        const uint32_t index = entity.Index();
        if (index >= slots.size()) return false;

        const Slot& slot = slots[index];
        return slot.alive && (slot.generation == entity.Generation());
    }

    void EntityManager::Reset()
    {
        slots.clear();
        freeHead = Invalid;
    }
}