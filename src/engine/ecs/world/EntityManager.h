#ifndef TERRANENGINE_ENTITYMANAGER_H
#define TERRANENGINE_ENTITYMANAGER_H

#include "engine/ecs/Entity.h"

#include <cstdint>
#include <vector>

namespace TerranEngine
{
    /**
     * Entity Manager controls the lifetime and storage of Entity handles.
     * 
     * ### Entity Storage.
     * 
     * A vector of `slots` stores all allocated entities, both live and free.
     * Each Entity `slot` holds a reference to the next free Entity in the array, resulting in an emergent singly-linked list of freed Entities, allowing for easy reuse of free Entity Indexes.
     * Entities are lazy-loaded, and prefer replacement of freed IDs over creating new Entities.
     */
    class EntityManager
    {
    public:
        EntityManager()  = default;
        ~EntityManager() = default;
        EntityManager(const EntityManager&)            = delete;
        EntityManager& operator=(const EntityManager&) = delete;

        [[nodiscard]] Entity CreateEntity();

        void DestroyEntity(Entity entity);
        [[nodiscard]] bool IsAlive(Entity entity) const noexcept;

        void Reset();

    private:
        struct Slot
        {
            uint32_t generation {0};
            uint32_t nextFree   {Invalid};
            bool     alive      {false};
        };

        std::vector<Slot> slots;
        uint32_t          freeHead {Invalid};

        static constexpr uint32_t Invalid        = 0xFFFFFFFFu;
        static constexpr uint32_t GenerationMask = 0xFFu;
    };
}

#endif // TERRANENGINE_ENTITYMANAGER_H