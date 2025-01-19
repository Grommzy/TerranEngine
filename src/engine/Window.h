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

    // Swaps the internal render buffers to show the underlying render-work.
    void SwapBuffers();

    // Returns the internal SDL_Window
    SDL_Window* GetSDLWindow() const;

    // Returns the window ID of the internal SDL_Window
    SDL_WindowID GetWindowID() const;

    // Returns the width of the window in pixels.
    int GetWidth() const;

    // Returns the height of the window in pixels.
    int GetHeight() const;

private:
    SDL_Window* window;
    SDL_WindowID windowID;
    int width;
    int height;
};

#endif // WINDOW_H