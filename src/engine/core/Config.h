#ifndef TERRANENGINE_CONFIG_H
#define TERRANENGINE_CONFIG_H

#include <string>

namespace TerranEngine
{
    struct Config
    {
        std::string title        {"TerranEngine"};
        int         nativeWidth  {480};
        int         nativeHeight {270};
        int         windowWidth  {1280};
        int         windowHeight {720};
        bool        resizable    {false};
        bool        vsync        {false};
        bool        growViewport {true};
    };
}

#endif // TERRANENGINE_CONFIG_H