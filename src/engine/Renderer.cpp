#include "engine/Renderer.h"

#include "engine/Window.h"

Renderer::Renderer()
{
    glContext = nullptr;
    isInitiliased = false;
}

Renderer::~Renderer()
{
    if (glContext)
    {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
}

bool Renderer::Init(Window window)
{
    
}