#include "engine/core/Application.h"

#include "engine/ecs/ScriptSystem.h"
#include "engine/ecs/CameraSystem.h"
#include "engine/gfx/SpriteRenderer.h"

#include <algorithm>

namespace TerranEngine
{
    Application::Application(const Config& config) noexcept
    {
        if (!windowManager.Initialise(config))
        {
            TE_LOG_ERROR("Window Manager initialisation failed.");
            std::abort();
        }

        world = std::make_unique<World>();

        world->AddSystem<ScriptSystem>();
        world->AddSystem<SpriteRenderer>(SystemPhase::RENDER, 0);
        world->AddSystem<CameraSystem>(SystemPhase::UPDATE, 0, windowManager);

        Time::Init();
        running = true;

        TE_LOG_INFO("Initialisation complete ({}x{} native-resolution | {}x{} window-resolution).", config.nativeWidth, config.nativeHeight, config.windowWidth, config.windowHeight);
    }

    void Application::Run() noexcept
    {
        while (running)
        {
            Tick();
        }

        Shutdown();
    }

    void Application::Tick() noexcept
    {
        PollEvents();

        Time::Tick();
        const float deltaTime = Time::DeltaTime();

        windowManager.BeginFrame();

        world->UpdateSystems(deltaTime);
        windowManager.EndFrame();

        
    }

    void Application::PollEvents() noexcept
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT: { running = false; break; }
                case SDL_EVENT_WINDOW_RESIZED: { windowManager.BeginFrame(); break; }
                default: { break; }
            }
        }
    }

    void Application::Shutdown() noexcept
    {
        TE_LOG_INFO("Shutting down application...");

        windowManager.Shutdown();
        world.reset();
        running = false;

        TE_LOG_INFO("Goodbye!");
    }
}