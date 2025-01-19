#include "engine/GLContext.h"

#include <GLAD/glad.h>

GLContext::GLContext(SDL_Window* defaultWindow) : window(defaultWindow), context(nullptr)
{
    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
    }

    // GLAD function pointers must be loaded for every context that utilises OpenGL.
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        throw std::runtime_error("Failed to initialise GLAD function pointers: " + std::string(SDL_GetError()));
    }
}

GLContext::~GLContext()
{
    if (context)
    {
        SDL_GL_DestroyContext(context);
    }
}

void GLContext::MakeCurrent()
{
    if (!SDL_GL_MakeCurrent(window, context))
    {
        throw std::runtime_error("Failed to make OpenGL context current: " + std::string(SDL_GetError()));
    }
}

void GLContext::ClearCurrent()
{
    {
        SDL_GL_MakeCurrent(nullptr, nullptr);
    }
}

SDL_GLContext GLContext::GetContext()
{
    return context;
}