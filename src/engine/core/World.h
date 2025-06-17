#ifndef WORLD_H
#define WORLD_H

#include "engine/ecs/Registry.h"

#include <memory>
#include <vector>

namespace TerranEngine
{
    class ISystem
    {
    public:
        virtual void Update(float deltaTime) = 0;
        virtual ~ISystem() = default;
    };

    class World
    {
    public:
        template<typename System, typename... Args>
        System& AddSystem(Args&&... args)
        {
            auto system = std::make_unique<System>(std::forward<Args>(args)...);
            System& systemReference = *system;
            systems.emplace_back(std::move(system));
            return systemReference;
        }

        void Update(float deltaTime)
        {
            for (const std::unique_ptr<ISystem>& system : systems)
            {
                system->Update(deltaTime);
            }
        }

        [[nodiscard]] Registry&       GetRegistry()       noexcept { return registry; }
        [[nodiscard]] const Registry& GetRegistry() const noexcept { return registry; }

    private:
        std::vector<std::unique_ptr<ISystem>> systems;
        Registry registry;
    };
}

#endif // WORLD_H