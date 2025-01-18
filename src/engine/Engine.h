#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>

#include "engine/Window.h"

class Engine
{
public:
    Engine();
    ~Engine();

    // Initialises the SDL Window, OpenGL context, etc.
    bool Initialise(const std::string& windowTitle, int width, int height);

    // The main loop. Updates and renders every frame.
    void Run();

    // Cleans up resources and ends the engine.
    void Quit();
    
private:
    void ProcessEvents();

private:
    // Indicator that the engine is running.
    bool isRunning;

    // List of all windows with an OpenGL context.
    Window window;
};

#endif // ENGINE_H