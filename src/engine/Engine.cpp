#include "engine/Engine.h"

#include <iostream>
#include <glad/glad.h>

#include "engine/Window.h"

Engine::Engine()
{
    isRunning   = false;
}

Engine::~Engine()
{
    Quit();
}

bool Engine::Initialise(const std::string& windowTitle, int width, int height)
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Create SDL Window
    if (!window.Create(windowTitle.c_str(), width, height, false))
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

void Engine::Run()
{
    while(isRunning)
    {
        ProcessEvents();

        // Clear screen in black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.SwapBuffer();
    }
}

void Engine::Quit()
{
    isRunning = false;

    // All windows are destroyed as they are stack objects.
    SDL_Quit();
}

void Engine::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // Global Events
            case SDL_EVENT_QUIT:
            {
                isRunning = false;
                break;
            }
        }
    }
}