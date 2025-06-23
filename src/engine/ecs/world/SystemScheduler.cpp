#include "engine/ecs/world/SystemScheduler.h"
#include "engine/ecs/world/World.h"

namespace TerranEngine
{
    void SystemScheduler::UpdateAll(World& world, float deltaTime)
    {
        Clean();

        for (Entry& entry : entries)
        {
            entry.system->Update(world, deltaTime);
        }
    }
}