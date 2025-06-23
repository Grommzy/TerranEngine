#include "engine/ecs/ScriptSystem.h"

#include "engine/ecs/world/World.h"
#include "engine/ecs/Behaviour.h"
#include "engine/core/Time.h"

namespace TerranEngine
{
    void ScriptSystem::Update(World& world, float deltaTime)
    {
        // 1. Awake()
        world.ForEach<BehaviourComponent>([&world](Entity entity, BehaviourComponent& component)
        {
            if (!world.IsAlive(entity)) return;

            Behaviour* script = component.behaviour.get();
            if (!script->awake)
            {
                script->entity = entity;
                script->world  = &world;
                script->Awake();
                script->awake = true;
            }
        });

        // 2. Start()
        world.ForEach<BehaviourComponent>([&world](Entity entity, BehaviourComponent& component)
        {
            if (!world.IsAlive(entity)) return;

            Behaviour* script = component.behaviour.get();
            if (script->awake && !script->started)
            {
                script->Start();
                script->started = true;
            }
        });

        // 3. FixedUpdate()
        int fixedSteps = Time::ConsumeFixedTicks();
        float fixedDelta = Time::FixedDeltaTime();

        for (int i = 0; i < fixedSteps; ++i)
        {
            world.ForEach<BehaviourComponent>([&world, fixedDelta](Entity entity, BehaviourComponent& component)
            {
                if (!world.IsAlive(entity)) return;

                Behaviour* script = component.behaviour.get();
                if (script->started)
                {
                    script->FixedUpdate(fixedDelta);
                }
            });
        }

        // 4. Update()
        world.ForEach<BehaviourComponent>([&world, deltaTime](Entity entity, BehaviourComponent& component)
        {
            if (!world.IsAlive(entity)) return;

            Behaviour* script = component.behaviour.get();
            if (script->started)
            {
                script->Update(deltaTime);
            }
        });
    }
}