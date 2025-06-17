#include "engine/level/TileParser.h"

#include "engine/core/Log.h"
#include "engine/ecs/Components.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace TerranEngine
{
    TileParser::TileParser(TileGrid& tileGrid, Registry& registry, const Texture& atlas, int tilePx) : tileGrid(tileGrid), registry(registry), atlas(atlas), tilePx(tilePx), currentRow(0) {}

    bool TileParser::Load(std::string_view path)
    {
        std::ifstream file { std::string { path } };

        if (!file)
        {
            TE_LOG_ERROR("TileLoader: failed to open '{}'", path);
            return false;
        }

        std::vector<std::vector<TileID>> tempRows;
        std::string row;
        
        while (std::getline(file, row, ';'))
        {
            if (!ParseRow(row)) { return false; }
            ++currentRow;
        }

        const int width  = static_cast<int>(tileGrid.Width());
        const int height = static_cast<int>(tileGrid.Height());

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const TileID id = tileGrid.GetTile(x, y);

                // Check for empty tile.
                if (id < 0) { continue; }

                Entity entity = registry.CreateEntity();

                glm::vec2 position = tileGrid.GridToWorld({x, y}, tilePx);
                registry.Add<Transform2D>(entity, position);

                auto& sprite = registry.Add<SpriteComponent>(entity);
                sprite.texture    = &atlas;
                sprite.size       = { tilePx, tilePx };
                sprite.atlasIndex = id;
            }
        }

        TE_LOG_INFO("TileLoader: loaded '{}' ({}x{})", path, width, height);
        return true;
    }

    bool TileParser::ParseRow(std::string_view row)
    {
        std::stringstream stringStream { std::string { row } };
        std::string cell;
        int column = 0;

        // Ensure that the tile-grid is big enough.
        if (currentRow == 0)
        {
            // Count the columns.
            while (std::getline(stringStream, cell, ',')) { ++ column; }
            tileGrid.Resize(column, 1); // Provisional height = 1
            stringStream.clear();
            stringStream.seekg(0);
            column = 0;
        }

        while (std::getline(stringStream, cell, ','))
        {
            cell.erase(std::remove_if(cell.begin(), cell.end(), [](unsigned char c){ return std::isspace(c); }), cell.end());

            if (cell.empty()) { ++column; continue; }

            const TileID id = std::stoi(cell);
            if (currentRow == 0)
            {
                tileGrid.SetTile(column, 0, id);
            }
            else
            {
                if (currentRow >= tileGrid.Height())
                {
                    tileGrid.Resize(tileGrid.Width(), currentRow + 1);
                }
                tileGrid.SetTile(column, currentRow, id);
            }
            ++column;
        }
        return true;
    }
}