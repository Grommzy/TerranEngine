#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#include "engine/ecs/Entity.h"
#include "engine/ecs/IComponentPool.h"
#include <vector>

namespace TerranEngine
{
    /**
     * Compartmentalises the relationships between components of type `T` (Transform, Sprite, ...) and their Entity parents.
     * To add components to Entities, call Emplace in the associated ComponentPool.
     */
    template <typename T>
    class ComponentPool : public IComponentPool
    {
    public:
        explicit ComponentPool(size_t maxEntities) : sparse(maxEntities, Invalid) {}

        /** Add a component of type `T` to Entity `entity`, or replace it if `entity` already had one.
         * @param Entity The Entity object to insert component into.
         * @param Args Components can be constructed inline using templated arguments. */
        template<typename... Args>
        T& Emplace(Entity entity, Args&&... args)
        {
            const uint32_t entityID = entity.Index();

            // Check if the component already exists for the Entity.
            if (sparse[entityID] != Invalid)
            {
                // Component already exists: replace in-place.
                dense[sparse[entityID]] = T(std::forward<Args>(args)...);
                return dense[sparse[entityID]]; // Return created component.
            }

            // Insert new component.
            const uint32_t denseIndex = static_cast<uint32_t>(dense.size());
            sparse[entityID] = denseIndex;
            dense.emplace_back(std::forward<Args>(args)...); // Construct new component and place at the back (preserves contiguity).
            owners.emplace_back(entity);                     // Hold the owner-entity in parrallel for easy parent lookup.
            return dense.back();                             // Return created component.
        }

        bool Has(Entity entity) const noexcept
        {
            uint32_t entityID = entity.Index();
            return entityID < sparse.size() && sparse[entityID] != Invalid;
        }

        T& Get(Entity entity)
        {
            return dense[sparse[entity.Index()]];
        }

        const T& Get(Entity entity) const
        {
            return dense[sparse[entity.Index()]];
        }

        /** Remove a component of type `T` from an Entity.
         * Inherited from type-erased IComponentPool interface */
        void Remove(Entity entity) override
        {
            const uint32_t entityID = entity.Index();
            const uint32_t denseID  = sparse[entityID];

            if (denseID == Invalid) { return; }

            // We don't necessarily need to delete the component explicitly unless it's at the back.
            // Instead, we can just overwrite it with the back component, and delete the duplicate/hanging component.
            // This preserves contiguity of the dense array, as order doesn't matter.
            const uint32_t lastDenseID = static_cast<uint32_t>(dense.size() - 1);
            if (denseID != lastDenseID)
            {
                dense[denseID]  = std::move(dense[lastDenseID]);
                owners[denseID] = owners[lastDenseID];
                sparse[owners[denseID].Index()] = denseID;
            }

            dense.pop_back();
            owners.pop_back();
            sparse[entityID] = Invalid;
        }

        template<typename Func>
        void ForEach(Func&& function)
        {
            for (size_t i = 0; i < dense.size(); ++i)
            {
                function(owners[i], dense[i]);
            }
        }

    private:
        static constexpr uint32_t Invalid = UINT32_MAX;

        std::vector<T>        dense;
        std::vector<Entity>   owners;
        std::vector<uint32_t> sparse;
    };
}

#endif // COMPONENTPOOL_H