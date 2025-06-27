#ifndef TERRANENGINE_WINDOWMANAGER_H
#define TERRANENGINE_WINDOWMANAGER_H

#include "engine/core/Config.h"

#include <SDL3/SDL.h>
#include <glad/gl.h>

namespace TerranEngine
{
    class WindowManager
    {
    public:
        WindowManager() = default;
        ~WindowManager() { Shutdown(); }

        [[nodiscard]] bool Initialise(const Config& config);

        void BeginFrame();
        void EndFrame();
        
        void Shutdown();

        [[nodiscard]] SDL_Window*   Window()         const noexcept { return window; }
        [[nodiscard]] SDL_GLContext Context()        const noexcept { return glContext; }
        [[nodiscard]] int           WindowWidth()    const noexcept { return windowWidth; }
        [[nodiscard]] int           WindowHeight()   const noexcept { return windowHeight; }
        [[nodiscard]] int           NativeWidth()    const noexcept { return nativeWidth; }
        [[nodiscard]] int           NativeHeight()   const noexcept { return nativeHeight; }
        [[nodiscard]] int           ViewportWidth()  const noexcept { return viewportWidth; }
        [[nodiscard]] int           ViewportHeight() const noexcept { return viewportHeight; }

    private:
        void CreateNativeFramebuffer();
        void DestroyNativeFramebuffer() noexcept;

        static void GLAPIENTRY GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParameter);

    private:
        SDL_Window*   window         {nullptr};
        SDL_GLContext glContext      {nullptr};
  
        GLuint        nativeFBO      {0};
        GLuint        nativeColour   {0};
        GLuint        nativeDepth    {0};
  
        int           windowWidth    {0};
        int           windowHeight   {0};
        int           nativeWidth    {0};
        int           nativeHeight   {0};
  
        int           targetWidth    {0};
        int           targetHeight   {0};

        int           scale          {1};
        int           xOffset        {0};
        int           yOffset        {0};
        int           viewportWidth  {0};
        int           viewportHeight {0};

        bool          vsync          {false};
        bool          growViewport   {true};
    };
}

#endif // TERRANENGINE_WINDOWMANAGER_H