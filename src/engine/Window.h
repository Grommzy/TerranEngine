#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

class GLContext;    // Forward declaration of GLContext.

class Window
{
public:
    // Default empty Constructor.
    Window();

    // Creates an SDL Window.
    Window(const char* title, int width, int height, bool fullscreen = false);

    // Clean up window pointer.
    ~Window();

    // Binds the window's drawable surface to the OpenGL context. Must be unbound to draw to other surfaces, such as the default surface, or off-screen buffers.
    bool MakeCurrent(GLContext* context);

    // Returns the internal SDL_Window
    SDL_Window* GetSDLWindow();

    // Returns the width of the window in pixels.
    int GetWidth();

    // Returns the height of the window in pixels.
    int GetHeight();

private:
    SDL_Window* window;
    int width;
    int height;
};

#endif // WINDOW_H