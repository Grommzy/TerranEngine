#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <SDL3/SDL.h>
#include <stdexcept>
/**
 * The State Machine of the OpenGL environment. Maintains the state of the OpenGL pipeline, including:
 * - Active shaders
 * - Textures
 * - Buffer Objects
 * - Framebuffers
 * - Vertex Array Objects
 * - etc.
 * 
 * Switching between contexts means switching between entirely different OpenGL states.
 * 
 * Contexts must be associated to a default drawable surface, i.e., the main window (SDL_Window*).
 * This link ensures that OpenGL can send rendered output to a specific window.
 * 
 * This default framebuffer (window/off-screen buffer) can be overwritten to set a different default drawable surface.
 * A framebuffer can also be bound ontop of the default framebuffer through the context's state, allowing for off-screen buffers to be drawn to.
 * Contexts operate on the currently bound framebuffer, so to return to the default buffer, simply bind 0 to GL_FRAMEBUFFER.
 * 
 * Contexts can share resources like textures, buffers, and shaders, to facilitate multi-threaded applications without resource duplication.
 * Contexts are otherwise independent, allowing for multiple contexts to coexist simultaneously for multi-window rendering.
 */
class GLContext
{
public:
    // Creates the OpenGL Context for an associated window.
    GLContext(SDL_Window* associatedWindow);

    // Cleans up hanging context and window pointer.
    ~GLContext();

    // Makes this context the current working context.
    void MakeCurrent();

    // Empty the working OpenGL context. Must select a new context afterwards.
    static void ClearCurrent();

    // Returns the SDL_GLContext.
    SDL_GLContext GetContext();

private:
    SDL_GLContext context;
    SDL_Window* window;
};

#endif // GLCONTEXT_H