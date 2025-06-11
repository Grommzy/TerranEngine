#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL3/SDL.h>
#include <glad/gl.h>

#include <string_view>

namespace TerranEngine
{
    class Application
    {
    public:
        struct Config
        {
            int  nativeWidth   {480};
            int  nativeHeight  {270};
            int  windowWidth   {960};   // Initial window size (2x scale)
            int  windowHeight  {540};   // Initial window size (2x scale)
            bool resizable    {true};
            std::string_view title {"TerranEngine Demo"};
        };

    public:
        explicit Application(const Config& config); // Configured Constructor
        Application() : Application(Config{}) {}    // Default Constructor
        ~Application();                             // Destructor

        // Ensure that the application is non-copyable.
        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;

        bool PumpEvents();  // Returns false when user requests `Quit`.
        void BeginFrame();  // Bind native FBO, set viewport.
        void EndFrame();    // Blit to screen, swap buffers.

        [[nodiscard]] float DeltaTime() const noexcept { return deltaTime; }

    private:
        void CreateWindow(const Config& config);
        void CreateGLContext();
        void CreateNativeFramebuffer(int width, int height);
        void SetupGLDebugCallback();

    private:
        SDL_Window* window      {nullptr};
        SDL_GLContext glContext {nullptr};

        GLuint nativeFBO     {0};
        GLuint nativeTexture {0};

        int nativeWidth  {0};
        int nativeHeight {0};
        int windowWidth  {0};
        int windowHeight {0};

        uint64_t prevTicks {0};
        double   deltaTime {0.0};
    };
} // TerranEngine namespace.

#endif //APPLICATION_H