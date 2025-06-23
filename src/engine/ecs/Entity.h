#ifndef TERRANENGINE_ENTITY_H
#define TERRANENGINE_ENTITY_H

#include <cstdint>

namespace TerranEngine
{
    /**
     * ### Bit-Packing and Optimisation.
     * 
     * Entity IDs are stored using a packed 32-bit int with the following layout:
     * ```
     * [ Generation (8-bits) ][ Index (24-bits) ]
     * [31                 24][23              0]
     * ```
     * @param Generation: The Generation of the existence of this Index. Starting at zero, every time an Entity of an Index is destroyed, and another Entity recycles it's Index, it is differentiated by an incremented Generation.
     * @param Index: The unique identifier for the Entity. Once the Entity is deleted, it's Index can be recycled, using the Generation segment to differentiate it from the previous Entity.
     * 
     * This packing is used solely for recycling of IDs, allowing us to have 16,777,216 active Entities at any time, even if many more have been deleted.
     * 
     * ### Integration into ECS and World.
     * 
     * Components are stored in a Sparse-Set Dense Array Pool that allows us to retrieve Components through the Entity by calling dense[sparse[entity.index]].
     * This works by tightly packing Components inside of a dense array, and passing their index in the dense array as the value to the sparse array at the index of the parent Entity's Index.
     * This structure is used to preserve contiguity of Component storage, increasing cache efficiency when looping/retrieving.
     * 
     * Invalid/Null Entities are represented by `ID=0`
     */
    class Entity
    {
    public:
        constexpr Entity() noexcept = default;
        constexpr explicit Entity(uint32_t rawID) noexcept : id(rawID) {}

        [[nodiscard]] constexpr uint32_t Index()      const noexcept { return id & indexMask; }
        [[nodiscard]] constexpr uint32_t Generation() const noexcept { return (id >> indexBits) & generationMask; }
        [[nodiscard]] constexpr uint32_t Raw()        const noexcept { return id; }

        [[nodiscard]] constexpr bool operator==(Entity entity) const noexcept { return id == entity.id; }
        [[nodiscard]] constexpr bool operator!=(Entity entity) const noexcept { return id != entity.id; }
        [[nodiscard]] constexpr explicit operator bool()       const noexcept { return id != 0; }

        static constexpr uint32_t CreateEntity(uint32_t index, uint32_t generation) noexcept { return (generation << indexBits) | (index & indexMask); }

    private:
        uint32_t id {0};

        static constexpr uint32_t indexBits      {24};
        static constexpr uint32_t generationBits {8};
        static constexpr uint32_t indexMask      { (1u << indexBits) - 1u };
        static constexpr uint32_t generationMask { (1u << generationBits) - 1u };
    };
}

#endif // TERRANENGINE_ENTITY_H