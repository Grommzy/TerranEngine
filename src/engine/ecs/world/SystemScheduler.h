#ifndef TERRANENGINE_SYSTEMSCHEDULER_H
#define TERRANENGINE_SYSTEMSCHEDULER_H

#include "engine/ecs/System.h"

#include <memory>
#include <vector>
#include <algorithm>

namespace TerranEngine
{
    enum class SystemPhase : int
    {
        PREUPDATE  = 0,
        UPDATE     = 1,
        POSTUPDATE = 2,
        RENDER     = 3
    };

    /**
     * @brief System Scheduler manages the lifetime and storage of Systems (implementing interface `System`).
     * 
     * ### System Storage.
     * 
     * Systems are stored as `Entries`, which link Systems with their operating System Phase, and their priority within each System Phase.
     * These `Entries` are stored in a contiguous vector with a `dirty` flag. When the vector is updated, the vector needs 'cleaning'.
     * 
     * A `dirty` 'entries' vector indicates that the vector may not be in priority order; cleaning the vector sorts it into priority ONLY when it is flagged as dirty.
     * This allows the vector to be iterated through while preserving phase/priority without further sorting.
     */
    class SystemScheduler
    {
    public:
        template<typename T, typename... Args>
        T& Add(SystemPhase systemPhase, int priority, Args&&... args)
        {

            static_assert(std::is_base_of_v<System, T>, "Must derive from System.");

            auto system = std::make_unique<T>(std::forward<Args>(args)...);

            entries.emplace_back(Entry {std::move(system), systemPhase, priority});

            dirty = true;
            return *static_cast<T*>(entries.back().system.get());
        }

        template<typename T, typename... Args>
        T& Add(Args&&... args)
        {
            return Add<T>(SystemPhase::UPDATE, 0, std::forward<Args>(args)...);
        }

        void UpdateAll(World& world, float deltaTime);

        void Reset() { entries.clear(); }

    private:
        void Clean()
        {
            if (!dirty) return;

            std::ranges::stable_sort(entries, 
                [](const Entry& a, const Entry& b)
                {
                    if   (a.systemPhase != b.systemPhase) { return a.systemPhase < b.systemPhase; }
                    else { return a.priority < b.priority; }
                });

            dirty = false;
        }

    private:
        struct Entry
        {
            std::unique_ptr<System> system;
            SystemPhase             systemPhase;
            int                     priority;
        };

        std::vector<Entry> entries;
        bool dirty {false};
    };
}

#endif // TERRANENGINE_SYSTEMSCHEDULER_H