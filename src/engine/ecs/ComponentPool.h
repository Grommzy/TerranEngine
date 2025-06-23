#ifndef TERRANENGINE_COMPONENTPOOL_H
#define TERRANENGINE_COMPONENTPOOL_H

#include "engine/ecs/Entity.h"

#include <vector>

namespace TerranEngine
{
    /** Non-templated interface so pools can be stored heterogenously. */
    class IComponentPool
    {
    public:
        virtual ~IComponentPool() = default;
        virtual void Remove(Entity entity) noexcept = 0;
        virtual bool Has(Entity entity) const noexcept = 0;
    };

    /**
     * @brief Templated pool that stores all components of type T in the `World`.
     * 
     * ### Data Structure.
     * 
     * `ComponentPool` stores a homogenous vector of a single Component type mapped to the Entities that own each component.
     * It uses a sparse-set dense array pool to retain contiguity in component storage for cache optimisations.
     * 
     * ```
     * [   entity2,     entity7,   entity36, ...] (Entity Array)
     * [component0, component45, component7, ...] (Dense Array)
     * 
     * [component0.index, ..., component7.index, ..., component45.index, ...] (Sparse Array)
     * ```
     * 
     * @param Dense:  Stores component objects in a contiguous vector. Component's index in the `dense array` is passed as the value at the index of it's parent Entity's Index in the `sparse array`.
     * @param Sparse: Stores the `dense index` of the child Component at the index of a parent Entity's Index. This allows us to retrieve Components through the parent Entity.
     * @param Entity: Stores the parent entity in parallel with it's child Component, allowing for quick lookups of both objects.
     */
    template<typename T>
    class ComponentPool final : public IComponentPool
    {
    public:
        T& Add(Entity entity, T&& component)
        {
            // Ensure that the sparse array has enough space to fit a new Entity Index.
            // Retrieve the index of the final element to preserve contiguity (size() records in 1-indexing, meaning it translates intrinsically to 0-indexing).
            ResizeToFit(entity.Index());
            const uint32_t denseIndex = static_cast<uint32_t>(denseData.size());

            // Place Component contiguously at the back of the dense array in parallel with it's parent Entity.
            // Record it's position in the sparse array at the index of it's parent Entity's Index.
            denseData.emplace_back(std::move(component));
            denseEntities.emplace_back(entity.Raw());
            sparseIndices[entity.Index()] = denseIndex;
            
            return denseData.back();
        }

        template<typename... Args>
        T& Emplace(Entity entity, Args&&... args)
        {
            // Ensure that the sparse array has enough space to fit a new Entity Index.
            // Retrieve the index of the final element to preserve contiguity (size() records in 1-indexing, meaning it translates intrinsically to 0-indexing).
            ResizeToFit(entity.Index());
            const uint32_t denseIndex = static_cast<uint32_t>(denseData.size());

            // Build component using forwarded arguments, then place it contiguously at the back of the dense array in parallel with it's parent Entity.
            // Record it's position in the sparse array at the index of it's parent Entity's Index.
            denseData.emplace_back(std::forward<Args>(args)...);
            denseEntities.emplace_back(entity.Raw());
            sparseIndices[entity.Index()] = denseIndex;

            return denseData.back();
        }

        void Remove(Entity entity) noexcept override
        {
            const uint32_t entityID = IndexOf(entity);
            const uint32_t denseID  = sparseIndices[entityID];

            if (denseID == Invalid) { return; }

            // We don't necessarily need to delete the component explicitly unless it's at the back.
            // Instead, we can just overwrite it with the back component, and delete the duplicate/hanging component.
            // This preserves contiguity of the dense array, as order doesn't matter.
            const uint32_t lastDenseID = static_cast<uint32_t>(denseData.size() - 1);
            if (denseID != lastDenseID)
            {
                denseData[denseID]  = std::move(denseData[lastDenseID]);
                denseEntities[denseID] = denseEntities[lastDenseID];
                sparseIndices[denseEntities[denseID].Index()] = denseID;
            }

            denseData.pop_back();
            denseEntities.pop_back();
            sparseIndices[entityID] = Invalid;
        }

        [[nodiscard]] bool     Has(Entity entity) const noexcept override  { const uint32_t index = IndexOf(entity); return index != Invalid && denseEntities[index] == entity; }
        [[nodiscard]] const T* Get(Entity entity) const noexcept           { const uint32_t index = IndexOf(entity); return (index == Invalid) ? nullptr : &denseData[index]; }
        [[nodiscard]] T*       Get(Entity entity) noexcept                 { const uint32_t index = IndexOf(entity); return (index == Invalid) ? nullptr : &denseData[index]; }
        [[nodiscard]] const std::vector<T>& Data() const noexcept          { return denseData; }
        [[nodiscard]] const std::vector<Entity>& Entities() const noexcept { return denseEntities; }
    
    private:
        void ResizeToFit(uint32_t index) { if (index >= sparseIndices.size()) { sparseIndices.resize(index + 1u, Invalid); } }
        [[nodiscard]] uint32_t IndexOf(Entity entity) const noexcept { return (entity.Index() < sparseIndices.size()) ? sparseIndices[entity.Index()] : Invalid; }

    private:
        std::vector<T> denseData;
        std::vector<Entity> denseEntities;
        std::vector<uint32_t> sparseIndices;

        static constexpr uint32_t Invalid = 0xFFFFFFFFu;
    };
}

#endif // TERRANENGINE_COMPONENTPOOL_H