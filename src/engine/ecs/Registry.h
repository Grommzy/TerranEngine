#ifndef REGISTRY_H
#define REGISTRY_H

#include "engine/ecs/Entity.h"
#include "engine/ecs/ComponentPool.h"

#include <memory>
#include <unordered_map>
#include <queue>
#include <typeindex>

namespace TerranEngine
{
    class Registry
    {
    public:
        explicit Registry(size_t maxEntities = 1 << 20) : maxEntities(maxEntities) {}

        Entity CreateEntity()
        {
            uint32_t entityID;
            uint32_t generation;

            if (!freeList.empty())
            {
                entityID = freeList.front();
                freeList.pop();
                generation = generations[entityID] + 1;
                generations[entityID] = generation;
            }
            else
            {
                entityID = static_cast<uint32_t>(generations.size());
                generations.push_back(0);
                generation = 0;
            }

            // Entity ID format:
            // [ Generation (12-bits) ][ Index (20-bits) ]
            // [31                  20][19              0]
            return Entity((generation << 20) | entityID);
        }

        void DestroyEntity(Entity entity)
        {
            const uint32_t entityID = entity.Index();
            if (entityID >= generations.size()) { return; }

            // Remove the entity from all component pools.
            for (auto& [key, pool] : pools)
            {
                pool->Remove(entity);
            }

            freeList.push(entityID);
        }

        template<typename T, typename... Args>
        T& Add(Entity entity, Args&&... args)
        {
            return Pool<T>().Emplace(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        bool Has(Entity entity) const
        {
            auto iterator = pools.find(std::type_index(typeid(T)));
            return (iterator != pools.end()) && static_cast<ComponentPool<T>*>(iterator->second.get())->Has(entity);
        }

        template<typename T>
        T& Get(Entity entity)
        {
            return Pool<T>().Get(entity);
        }

        template<typename First, typename... Rest, typename Func>
        void ForEach(Func&& func)
        {
            auto& firstPool = Pool<First>();

            firstPool.ForEach([&](Entity ent, First& firstComp)
            {
                if ((Has<Rest>(ent) && ...))          // entity owns all Rest comps
                {
                    func(ent, firstComp, Get<Rest>(ent)...);  // no duplicate
                }
            });
        }

    private:
        template<typename T>
        ComponentPool<T>& Pool()
        {
            std::type_index key = std::type_index(typeid(T));
            auto iterator = pools.find(key);
            if (iterator == pools.end())
            {
                auto pool = std::make_unique<ComponentPool<T>>(maxEntities);
                auto* raw = pool.get();
                pools.emplace(key, std::move(pool));

                return *raw;
            }

            return *static_cast<ComponentPool<T>*>(iterator->second.get());
        }

        template<typename T>
        const ComponentPool<T>& Pool() const
        {
            std::type_index key = std::type_index(typeid(T));
            return *static_cast<const ComponentPool<T>*>(pools.at(key).get());
        }

    private:
        size_t maxEntities;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;

        std::vector<uint32_t> generations;
        std::queue<uint32_t>  freeList;
    };
}

#endif // REGISTRY_H