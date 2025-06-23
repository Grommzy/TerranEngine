#ifndef TERRANENGINE_APPLICATION_H
#define TERRANENGINE_APPLICATION_H

#include <string_view>
#include <SDL3/SDL.h>
#include <glad/gl.h>

#include "engine/core/Time.h"
#include "engine/core/Log.h"
#include "engine/ecs/world/World.h"

namespace TerranEngine
{
    /** Root coordinator for game initialisation, main-loop, and shutdown. */
    class Application
    {
    public:
        struct Config
        {
            int  nativeWidth   {480}; // Width in pixels.
            int  nativeHeight  {270}; // Height in pixels.
            int  windowWidth   {960}; // Resolution of window.
            int  windowHeight  {540}; // Resolution of window.
            bool resizable     {true};
            std::string_view title {"TerranEngine"};
        };

        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&)                 = delete;
        Application& operator=(Application&&)      = delete;

        ~Application() { Shutdown(); }

        /** Construct TerranEngine with desired window size and title. */
        explicit Application(const Config& config) noexcept;

        /** GCC Bug requires this vestigial default constructor */
        Application() : Application(Config{}) {}

        /** Grab the world from the Application. */
        [[nodiscard]] World& GetWorld() noexcept { return *world; }

        void SetActiveWorld(std::unique_ptr<World> world) noexcept { this->world = std::move(world); }

        /** Enter the main loop. Returns when game has been quit. */
        void Run() noexcept;

        /** Tear down subsystems. */
        void Shutdown() noexcept;

        /** Returns true if the Application is running. */
        [[nodiscard]] bool IsRunning() const noexcept { return running; }

        /** TODO: Reroute this functionality into a dedicated rendering system. */
        void BeginFrame() noexcept;
        void EndFrame()   noexcept;

    private:
        /** Per-frame updates. */
        void Tick() noexcept;

        /** TODO: TEMPORARILY handle SDL events before EventManager. */
        void PollEvents() noexcept;

        void CreateWindow(const Config& config);
        void CreateGLContext();
        void CreateNativeFramebuffer(int width, int height);
        void SetupGLDebugCallback();

    private:
        // --- Window state --- //
        SDL_Window*   window    {nullptr};
        SDL_GLContext glContext {nullptr};

        GLuint nativeFBO {0};
        GLuint nativeTex {0};

        int nativeWidth  {0};
        int nativeHeight {0};
        int windowWidth  {0};
        int windowHeight {0};

        bool running {false};

        std::unique_ptr<World> world;
    };
}

#endif // TERRANENGINE_APPLICATION_H