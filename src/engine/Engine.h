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
    // Indicator that the engine is running.
    bool isRunning;

    // Reference to the currently focused window for event handling.
    Window focusedWindow;

    // List of all windows with OpenGL flags.
    std::map<SDL_WindowID, Window*> windows;

    // List of GLContexts mapped to their default drawable surface.
    std::map<Window*, GLContext*> contexts;
};

#endif // ENGINE_H