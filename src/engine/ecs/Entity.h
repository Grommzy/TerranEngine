#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

namespace TerranEngine
{
    /** Entity IDs are stored using a packed 32-bit int with the following layout:
     * ```
     * [ Generation (12-bits) ][ Index (20-bits) ]
     * [31                  20][19              0]
     * ```
     * @param Generation: The Generation of the existence of this Index. Starting at zero, every time an Entity of an Index is destroyed, and another Entity recycles it's Index, it is differentiated by an incremented Generation.
     * @param Index: The unique identifier for the Entity. Once the Entity is deleted, it's Index can be recycled, using the Generation segment to differentiate it from the previous Entity.
     * 
     * This packing is used solely for recycling of IDs, allowing us to have 1,048,575 active Entities at any time, even if many more have been deleted.
     * 
     * Entites are stored in a sparse-dense array system. Entities are stored in a `dense array` at an incrementing dIndex. This dIndex is stored inside of a `sparse array` at an Index of the Entity.
     * This results in a `map` structure with O(1) lookups using ints instead of string comparisons, which can retrieve Entities by searching `dense[sparse[EntityIndexID]]` (int EntityID -> position in Entity array -> Entity entity).
     * 
     * Invalid/Null Entities are represented by `ID=0`
     */
    class Entity
    {
    public:
        constexpr Entity() : id(0) {}
        constexpr explicit Entity(uint32_t raw) : id(raw) {}

        [[nodiscard]] uint32_t Index()      const noexcept { return id & IndexMask; }
        [[nodiscard]] uint32_t Generation() const noexcept { return id >> GenShift; }
        [[nodiscard]] bool     Valid()      const noexcept { return id != 0; }

        constexpr bool operator==(Entity other) const noexcept { return id == other.id; }
        constexpr bool operator!=(Entity other) const noexcept { return id != other.id; }
        constexpr operator uint32_t()           const noexcept { return id; }

    private:
        static constexpr uint32_t IndexMask = (1u << 20) - 1; // Lower 20 bits.
        static constexpr uint32_t GenShift = 20;

        uint32_t id;
    };

    inline constexpr Entity NullEntity {};
}

#endif // ENTITY_H