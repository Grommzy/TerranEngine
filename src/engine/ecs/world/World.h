#ifndef TERRANENGINE_WORLD_H
#define TERRANENGINE_WORLD_H

#include "engine/ecs/world/EntityManager.h"
#include "engine/ecs/world/ComponentManager.h"
#include "engine/ecs/world/SystemScheduler.h"
#include "engine/ecs/world/QueryEngine.h"

namespace TerranEngine
{
    /**
     * @brief World acts as a facade for the underlying `ComponentManager`, `EntityManager`, `SystemScheduler`, and `QueryEngine` classes, abstracting them away from the public interface.
     */
    class World
    {
    public:
        World() : querier{components} {}
        ~World() = default;

        [[nodiscard]] Entity CreateEntity() { return entities.CreateEntity(); }
        void DestroyEntity(Entity entity) { entities.DestroyEntity(entity); }
        [[nodiscard]] bool IsAlive(Entity entity) const { return entities.IsAlive(entity); }

        template<typename T, typename... Args>
        T& AddComponent(Entity entity, Args&&... args) { return components.Add<T>(entity, std::forward<Args>(args)...); }

        template<typename T>
        bool RemoveComponent(Entity entity) { return components.Remove<T>(entity); }

        template<typename T>
        [[nodiscard]] T* GetComponent(Entity entity) { return components.Get<T>(entity); }

        template<typename T>
        [[nodiscard]] const T* GetComponent(Entity entity) const { return components.Get<T>(entity); }

        template<typename T>
        [[nodiscard]] bool HasComponent(Entity entity) const { return components.Has<T>(entity); }

        /** Function/Lambda `must` parse Entity first, and then non-const references to the components in the same order that they were given in the template list. */
        template<typename... Components, typename Function>
        void ForEach(Function&& function) { querier.ForEach<Components...>(std::forward<Function>(function)); }

        template<typename System, typename... Args>
        System& AddSystem(SystemPhase phase = SystemPhase::UPDATE, int priority = 0, Args&&... args) { return scheduler.Add<System>(phase, priority, std::forward<Args>(args)...); }

        void UpdateSystems(float deltaTime) { scheduler.UpdateAll(*this, deltaTime); }

        void Clear()
        {
            scheduler.Reset();
            components.Reset();
            entities.Reset();
        }

    private:
        EntityManager    entities;
        ComponentManager components;
        SystemScheduler  scheduler;
        QueryEngine      querier;
    };
}

#endif // TERRANENGINE_WORLD_H