#include "engine/gfx/AnimationSystem.h"

#include "engine/ecs/Components.h"

namespace TerranEngine
{
    void AnimationSystem::Update(float deltaTime)
    {
        auto& registry = world.GetRegistry();

        registry.ForEach<AnimationComponent, SpriteComponent>([&](Entity, AnimationComponent& animation, SpriteComponent& sprite)
            {
                if (animation.frames.empty())
                {
                    return;
                }

                animation.timer += deltaTime;
                if (animation.timer >= animation.frameTime)
                {
                    animation.timer -= animation.frameTime;
                    animation.cursor =
                        (animation.cursor + 1) %
                        animation.frames.size();
                    sprite.atlasIndex =
                        animation.frames[animation.cursor];
                }
            });
    }
}
