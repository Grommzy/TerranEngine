#include "engine/Engine.h"

#include <iostream>
#include <glad/glad.h>

#include "engine/Window.h"
#include "engine/GLContext.h"

Engine::Engine()
{
    isRunning = false;
}

Engine::~Engine()
{
    isRunning = false;

    SDL_Quit();
}

bool Engine::Init(const char* windowTitle, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set initial OpenGL Attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    CreateWindow("Terran Engine", 200, 600);
    CreateWindow("AnotherWindow", 600, 200);

    return true;
}

void Engine::Run()
{
    windows.at(0)->MakeCurrent(contexts[windows.at(0)]);
    
    while (true)
    {
        for (Window* window : windows)
        {
            // Make the window the current drawable surface.
            // Set the glViewport rendering portion to the whole window (fill the whole window)
            window->MakeCurrent(contexts[window]);
            glViewport(0, 0, window->GetWidth(), window->GetHeight());
        }
    }
}

bool Engine::CreateWindow(const char* windowTitle, int width, int height)
{
    // Create default drawable surface (window)
    Window* window = new Window(windowTitle, width, height);
    windows.push_back(window);

    // Create the initial context, pointing to the window as the default drawable surface.
    GLContext* context = new GLContext(window->GetSDLWindow());
    contexts.emplace(window, context);

    return true;
}