#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

class Context;  // Forward declaration of Context 

class Window
{
public:
    // Creates an SDL Window.
    Window(const char* title, int width, int height, bool fullscreen = false);

    // Clean up window pointer.
    ~Window();

    // Returns the internal SDL_Window
    SDL_Window* GetSDLWindow();

private:
    SDL_Window* window;
};

#endif // WINDOW_H