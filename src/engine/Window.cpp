#include "engine/Window.h"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h>

Window::Window(const char* title, int width, int height, bool fullscreen)
{
    // Set window flags to support OpenGL contexts and fullscreen/windowed.
    SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL;
    if (fullscreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(title, width, height, windowFlags);
    if (!window)
    {
        throw std::runtime_error("Failed to create SDL_Window: " + std::string(SDL_GetError()));
    }

    // Set the viewport to match the created window.
    glViewport(0, 0, width, height);
}

Window::~Window()
{
    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

SDL_Window* Window::GetSDLWindow()
{
    return window;
}