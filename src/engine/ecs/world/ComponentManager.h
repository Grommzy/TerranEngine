#ifndef TERRANENGINE_COMPONENTMANAGER_H
#define TERRANENGINE_COMPONENTMANAGER_H

#include "engine/ecs/ComponentPool.h"

#include <unordered_map>
#include <typeindex>
#include <memory>

namespace TerranEngine
{
    /**
     * @brief Component Manager controls the storage of templated Component handles.
     * 
     * ### Component Storage
     * 
     * All Components of type T are already stored in templated `Component Pools`.
     * These `Component Pools` are then stored inside of the Component Manager, mapped against their `type-index`.
     * Mapping against the `type-index` provides us with a standard way to turn our templated compile-time types into unique run-time keys.
     */
    class ComponentManager
    {
    public:
        ComponentManager()  = default;
        ~ComponentManager() = default;

        ComponentManager(const ComponentManager&)           = delete;
        ComponentManager operator=(const ComponentManager&) = delete;

        template<typename T, typename... Args>
        T& Add(Entity entity, Args&&... args)
        {
            return GetOrCreatePool<T>().Emplace(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        bool Remove(Entity entity)
        {
            auto* pool = GetPool<T>();
            return pool ? (pool->Remove(entity), true) : false; // 'nullptr' is truthy = false.
        }

        template<typename T>
        [[nodiscard]] T* Get(Entity entity)
        {
            auto* pool = GetPool<T>();
            return pool ? pool->Get(entity) : nullptr; // 'nullptr' is truthy = false.
        }

        template<typename T>
        [[nodiscard]] const T* Get(Entity entity) const
        {
            auto* pool = GetPool<T>();
            return pool ? pool->Get(entity) : nullptr; // 'nullptr' is truthy = false.
        }

        template<typename T>
        [[nodiscard]] bool Has(Entity entity) const
        {
            const auto* pool = GetPool<T>();
            return pool ? pool->Has(entity) : false; // 'nullptr' is truthy = false.
        }

        template<typename T>
        [[nodiscard]] ComponentPool<T>* GetPool()
        {
            const auto iterator = pools.find(std::type_index{typeid(T)});
            return (iterator == pools.end()) ? nullptr : static_cast<ComponentPool<T>*>(iterator->second.get());
        }

        template<typename T>
        [[nodiscard]] const ComponentPool<T>* GetPool() const
        {
            const auto iterator = pools.find(std::type_index{typeid(T)});
            return (iterator == pools.end()) ? nullptr : static_cast<const ComponentPool<T>*>(iterator->second.get());
        }

        void Reset() { pools.clear(); }

    private:
        template<typename T>
        ComponentPool<T>& GetOrCreatePool()
        {
            const std::type_index typeIndex {typeid(T)};

            // Iterate across 'pools' to find the templated type. If it's missing, append a new Component Pool to the dictionary.
            auto iterator = pools.find(typeIndex);

            if (iterator == pools.end())
            {
                // Capture the returned iterator (and discard the boolean flag) to avoid re-hashing.
                auto [newIterator, _] = pools.emplace(typeIndex, std::make_unique<ComponentPool<T>>());

                return *static_cast<ComponentPool<T>*>(newIterator->second.get());
            }
            return *static_cast<ComponentPool<T>*>(iterator->second.get());
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;
    };
}

#endif // TERRANENGINE_COMPONENTMANAGER_H