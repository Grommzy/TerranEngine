#ifndef TILEPARSER_H
#define TILEPARSER_H

#include "engine/core/World.h"
#include "engine/level/TileGrid.h"
#include "engine/ecs/Registry.h"
#include "engine/gfx/Texture.h"

#include <string_view>

namespace TerranEngine
{
    class TileParser : public ISystem
    {
    public:
        TileParser(TileGrid& tileGrid, Registry& registry, const Texture& atlas, int tilePx = 16);

        bool Load(std::string_view path);

        void Update(float deltaTime) override {}

    private:
        bool ParseRow(std::string_view row);

    private:
        TileGrid& tileGrid;
        Registry& registry;
        const Texture& atlas;
        int tilePx;
        int currentRow {0};
    };
}

#endif // TILEPARSER_H