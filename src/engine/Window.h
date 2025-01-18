#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

class Context;  // Forward declaration of Context 

class Window
{
public:
    Window();
    ~Window();

    // Creates an SDL Window without a rendering context.
    bool Create(const char* title, int width, int height, bool fullscreen = false);

    // Polls for OS messages (close, keyboard events, etc.)
    void PollEvents();

    // Returns true if the user has asked to close the window.
    bool WillClose() const;

    // Swaps the rendering buffers.
    void SwapBuffer();

private:
    SDL_Window* window;
    SDL_GLContext glContext;
    bool isClosed;
};

#endif // WINDOW_H