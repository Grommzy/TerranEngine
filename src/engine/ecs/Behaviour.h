#ifndef TERRANENGINE_BEHAVIOUR_H
#define TERRANENGINE_BEHAVIOUR_H

#include "engine/ecs/Entity.h"

#include <memory>

namespace TerranEngine
{
    class World; // Forward declaration of World.

    /** @brief Base class for scriptable behaviours attached to Entities. */
    class Behaviour
    {
    public:
        virtual ~Behaviour() = default;

        /** Called when the script is first attached. Used in-place of a Constructor for initialisation. */
        virtual void Awake() {}

        /** Called once after Awake, before the first Update. */
        virtual void Start() {}

        /** Called every frame alongside each render-step. */
        virtual void Update(float deltaTime) {}

        /** Called every fixed update (at a 60hz interval). */
        virtual void FixedUpdate(float fixedDeltaTime) {}

        [[nodiscard]] Entity GetEntity() const noexcept { return entity; }
        [[nodiscard]] World& GetWorld()  const noexcept { return *world; }

    protected:
        Entity entity;
        World* world {nullptr};

    private:
        bool awake   {false};
        bool started {false};
        friend class ScriptSystem;

    };

    /** ECS Component that attaches a Behaviour (script instance) to an Entity. */
    struct BehaviourComponent
    {
        std::unique_ptr<Behaviour> behaviour;

        explicit BehaviourComponent(std::unique_ptr<Behaviour> behaviourPointer) : behaviour(std::move(behaviourPointer)) {}
    };
}

#endif // TERRANENGINE_BEHAVIOUR_H