#include "engine/Engine.h"

#include <iostream>
#include <glad/glad.h>

#include "engine/Window.h"
#include "engine/GLContext.h"

bool Engine::Init(const std::string& windowTitle, int width, int height)
{
    // Create default drawable surface (window)
    Window window("Terran Engine", 1920, 1080);
    Window anotherWindow("Another Engine", 400, 100);

    // Create the initial context, pointing to the window as the default drawable surface.
    GLContext context(window.GetSDLWindow());

    while(true) {}
    return true;
}