#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "engine/core/World.h"

namespace TerranEngine
{
    class AnimationSystem : public ISystem
    {
    public:
        explicit AnimationSystem(World& world) : world(world) {}

        void Update(float deltaTime) override;

    private:
        World& world;
    };
}

#endif // ANIMATIONSYSTEM_H
