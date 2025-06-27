#ifndef TERRANENGINE_APPLICATION_H
#define TERRANENGINE_APPLICATION_H

#include <memory>

#include "engine/core/Time.h"
#include "engine/core/Log.h"
#include "engine/core/Config.h"
#include "engine/ecs/world/World.h"
#include "engine/gfx/WindowManager.h"

namespace TerranEngine
{
    /** Root coordinator for game initialisation, main-loop, and shutdown. */
    class Application
    {
    public:
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

    private:
        /** Per-frame updates. */
        void Tick() noexcept;

        /** TODO: TEMPORARILY handle SDL events before EventManager. */
        void PollEvents() noexcept;

    private:
        // --- Window state --- //
        WindowManager          windowManager;
        std::unique_ptr<World> world;

        bool running {false};
    };
}

#endif // TERRANENGINE_APPLICATION_H