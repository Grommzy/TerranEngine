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
        template<typename... Components, typename Function>
        void ForEach(Function&& function)
        {
            static_assert((sizeof...(Components) > 0), "ForEach needs at least one component type.");

            // Pick the 0th entry in the list of parsed heterogenous components.
            using Lead = std::tuple_element_t<0, std::tuple<Components...>>;

            auto* leadPool = components.GetPool<Lead>();
            if (!leadPool) { return; }

            const auto& dense     = leadPool->Data();
            const auto& entityIDs = leadPool->Entities();

            for (size_t i = 0; i < dense.size(); ++i)
            {
                Entity entity {entityIDs[i]};

                if constexpr (sizeof...(Components) == 1)
                {
                    // Faster way of doing it for single-component cases.
                    if (components.Has<Lead>(entity)) { function(entity, const_cast<Lead&>(dense[i])); }
                }
                else
                {
                    if ((components.Has<Components>(entity) && ...))
                    {
                        function(entity, const_cast<Lead&>(dense[i]), *components.Get<Components>(entity)...);
                    }
                }
            }
        }

    private:
        ComponentManager& components;
    };
}

#endif // TERRANENGINE_QUERYENGINE_H