#include "engine/Window.h"

#include <iostream>
#include <stdexcept>
#include <glad/glad.h>

#include "engine/GLContext.h"

Window::Window()
{
    window = nullptr;
}

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

    this->width = width;
    this->height = height;
}

Window::~Window()
{
    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

bool Window::MakeCurrent(GLContext* context)
{
    if (!SDL_GL_MakeCurrent(this->GetSDLWindow(), context->GetContext()))
    {
        throw std::runtime_error("Failed to make window the current drawable surface: " + std::string(SDL_GetError()));
    }

    return true;
}

SDL_Window* Window::GetSDLWindow()
{
    return window;
}

int Window::GetWidth()
{
    return width;
}

int Window::GetHeight()
{
    return height;
}