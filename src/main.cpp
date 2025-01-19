#include <iostream>

#include "engine/Engine.h"

int main(int argc, char* argv[])
{
    Engine engine;

    if (engine.Init("Terran Engine", 200, 400))
    {
        engine.Run();
    }
    
    return 0;
}