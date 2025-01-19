#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <map>

#include "engine/Window.h"
#include "engine/GLContext.h"

class Engine
{
public:
    Engine();
    ~Engine();

    // Initialises the SDL Window, OpenGL context, etc.
    bool Init(const char* windowTitle, int width, int height);

    // The main loop. Updates and renders every frame.
    void Run();

    // Cleans up resources and ends the engine.
    void Quit();

    bool CreateWindow(const char* windowTitle, int width, int height);
    
private:
    void ProcessEvents();

private:
    // Indicator that the engine is running.
    bool isRunning;

    // List of all windows with OpenGL flags.
    std::vector<Window*> windows;

    // GLContext.
    std::map<Window*, GLContext*> contexts;
};

#endif // ENGINE_H