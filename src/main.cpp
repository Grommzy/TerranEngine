#include <iostream>

#include "engine/Engine.h"

int main(int argc, char* argv[])
{
    Engine engine;

    if (engine.Init("Terran Engine", 1920, 1080))
    {
        engine.Run();
    }
    
    return 0;
}