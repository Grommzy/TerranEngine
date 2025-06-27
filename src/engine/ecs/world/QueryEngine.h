#ifndef TERRANENGINE_QUERYENGINE_H
#define TERRANENGINE_QUERYENGINE_H

#include "engine/ecs/world/ComponentManager.h"

namespace TerranEngine
{
    class QueryEngine
    {
    public:
        explicit QueryEngine(ComponentManager& componentManager) : components(componentManager) {}

        /** Function/Lambda `must` parse Entity first, and then non-const references to the components in the same order that they were given in the template list. */
        template<typename Lead, typename... Rest, typename Function>
        void ForEach(Function&& function)
        {
            static_assert((sizeof...(Rest) >= 0), "ForEach needs at least one component type.");

            auto* leadPool = components.GetPool<Lead>();
            if (!leadPool) { return; }

            const auto& dense     = leadPool->Data();
            const auto& entityIDs = leadPool->Entities();

            for (size_t i = 0; i < dense.size(); ++i)
            {
                Entity entity {entityIDs[i]};

                if constexpr (sizeof...(Rest) == 0)
                {
                    // Faster way of doing it for single-component cases.
                    if (components.Has<Lead>(entity)) { function(entity, const_cast<Lead&>(dense[i])); }
                }
                else
                {
                    if ((components.Has<Rest>(entity) && ...))
                    {
                        function(entity, const_cast<Lead&>(dense[i]), *components.Get<Rest>(entity)...);
                    }
                }
            }
        }

    private:
        ComponentManager& components;
    };
}

#endif // TERRANENGINE_QUERYENGINE_H