#ifndef BHC_MAPPARSER_H
#define BHC_MAPPARSER_H

#include "engine/core/Log.h"

#include <string_view>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace TerranEngine;

namespace BlackHoleChest
{
    struct Map
    {
        uint16_t width  {0};
        uint16_t height {0};
        std::vector<uint16_t> mapTiles;
    };

    class MapParser
    {
    public:
        static bool ParseMap(const std::string_view& filePath, Map& outMap)
        {
            std::ifstream file {filePath.data()};

            if (!file)
            {
                TE_LOG_ERROR("MapParser: Failed to load '{}'.", filePath);
                return false;
            }

            Map map;
            std::string line;

            // 1. Retrieve file header values (Width, Height).
            if (!std::getline(file, line)) { TE_LOG_ERROR("MapParser: Missing width line in file '{}'", filePath); return false; }
            if (!ParseUint16tFromLine(line, map.width)) { TE_LOG_ERROR("MapParser: Invalid width value in file '{}'.", filePath); return false; }

            if (!std::getline(file, line)) { TE_LOG_ERROR("MapParser: Missing height line in file '{}'", filePath); return false; }
            if (!ParseUint16tFromLine(line, map.height)) { TE_LOG_ERROR("MapParser: Invalid height value in file '{}'.", filePath); return false; }

            // 2. Parse the tiles making up the map.
            const size_t mapSize = static_cast<size_t>(map.width * map.height);
            map.mapTiles.resize(mapSize);

            for (size_t i = 0; i < mapSize; ++i)
            {
                if (!std::getline(file, line)) { TE_LOG_ERROR("MapParser: Missing tile definition | {} / {}", i, map.width * map.height); return false; }
                if (!ParseUint16tFromLine(line, map.mapTiles[i])) { TE_LOG_ERROR("MapParser: Invalid Tile value in file '{}'.", filePath); return false; }
            }

            // 3. Output the new map struct into the injected input.
            outMap = std::move(map);
            return true;
        }

    private:
        [[nodiscard]] static bool ParseUint16tFromLine(const std::string& line, uint16_t& integer16)
        {
            int integer32;
            try
            {
                integer32 = std::stoi(line);
            }
            catch(const std::exception& exception)
            {
                TE_LOG_ERROR("Exception: {}", exception.what());
                return false;
            }

            if (integer32 < 0 || integer32 > std::numeric_limits<uint16_t>::max()) { TE_LOG_ERROR("MapParser: Invalid integer value in file."); return false;}
            
            integer16 = static_cast<uint16_t>(integer32);
            return true;
        };
    };
}

#endif // BHC_MAPPARSER_H