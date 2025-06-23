#ifndef TERRANENGINE_SCRIPTSYSTEM_H
#define TERRANENGINE_SCRIPTSYSTEM_H

#include "engine/ecs/System.h"

namespace TerranEngine
{
    class ScriptSystem : public System
    {
    public:
        ScriptSystem() = default;
        ~ScriptSystem() = default;

        void Update(World& world, float deltaTime) override;
    
    };
}

#endif // TERRANENGINE_SCRIPTSYSTEM_H