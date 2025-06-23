#ifndef TERRANENGINE_SYSTEM_H
#define TERRANENGINE_SYSTEM_H

namespace TerranEngine
{
    class World;

    /** Base class for polymorphic Systems integrated into the World. */
    class System
    {
    public:
        virtual ~System() = default;
        virtual void Update(World& world, float deltaTime) = 0;
    };
}

#endif // TERRANENGINE_SYSTEM_H