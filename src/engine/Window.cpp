#include "engine/Window.h"

#include <iostream>
#include <glad/glad.h>

Window::Window()
{
    window      = nullptr;
    glContext   = nullptr;
    isClosed    = false;
}

Window::~Window()
{
    // Clean up the OpenGL context and the SDL window.
    if (glContext)
    {
        SDL_GL_DestroyContext(glContext);
    }

    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

bool Window::Create(const char* title, int width, int height, bool fullscreen)
{
    // Create the Window
    Uint32 windowFlags = SDL_WINDOW_OPENGL;
    if (fullscreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow (
        title,
        width,
        height,
        windowFlags
    );

    if (!window)
    {
        std::cerr << "[ERROR] Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Use GLAD to load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "[Error] Failed to initialize GLAD." << std::endl;
        return false;
    }

    // Create an OpenGL context associated with the window.
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "[ERROR] OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set the viewport to match the created window
    glViewport(0, 0, width, height);

    isClosed = false;
    return true;
}

void Window::SwapBuffer()
{
    SDL_GL_SwapWindow(window);
}