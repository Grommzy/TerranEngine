#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

class Window;   // Forward declaration of Window

class Renderer
{
public:
    Renderer();
    ~Renderer();

    // Creates an OpenGL context for the given window, and loads OpenGL via. GLAD.
    bool Init(Window window);

    // Clear the screen with the chosen colour (RGBA).
    void Clear(float r, float g, float b, float a);

    // Perform rendering operations and swap buffers for the current window.
    void Render();

    // Returns true if the GL Context is valid/initialised.
    bool IsInitialised() const;
private:
    SDL_GLContext glContext;
    bool isInitiliased;
};

#endif // RENDERER_H