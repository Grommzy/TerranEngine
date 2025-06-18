#include "engine/core/Application.h"

#include <algorithm>

/** Activate OpenGL Debug Callbacks if enabled in the build configuration.
 * option(TE_ENABLE_GL_DEBUG   "Enable OpenGL debug Callbacks in Debug builds" ON) */
#if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
    namespace
    {
        void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* msg, const void*)
        {
            const char* sev =
                (severity == GL_DEBUG_SEVERITY_HIGH)   ? "HIGH"   :
                (severity == GL_DEBUG_SEVERITY_MEDIUM) ? "MEDIUM" :
                (severity == GL_DEBUG_SEVERITY_LOW)    ? "LOW"    : "NOTIFY";

            using namespace TerranEngine;
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:   TE_LOG_ERROR("[GL {}] {}", sev, msg); break;
                case GL_DEBUG_SEVERITY_MEDIUM: TE_LOG_WARN ("[GL {}] {}", sev, msg); break;
                default:                       TE_LOG_INFO ("[GL {}] {}", sev, msg); break;
            }
        }
    }
#endif

namespace TerranEngine
{
    Application::Application(const Config& config) noexcept
    {
        CreateWindow(config);
        CreateGLContext();
        CreateNativeFramebuffer(config.nativeWidth, config.nativeHeight);

        #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
            SetupGLDebugCallback();
        #endif

        Time::Init();
        running = true;

        TE_LOG_INFO("Initialisation complete ({}x{} native-resolution | {}x{} window-resolution).", config.nativeWidth, config.nativeHeight, config.windowWidth, config.windowHeight);    
    }

    void Application::Run() noexcept
    {
        if (!running) return;

        while (running)
        {
            Tick();
            BeginFrame();
            /** Draw stuff! */
            EndFrame();
        }
    }

    void Application::Tick() noexcept
    {
        Time::Tick();
        TE_LOG_DEBUG("FPS: {}", Time::FPS());
        PollEvents();
    }

    void Application::PollEvents() noexcept
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT: { running = false; break; }
                case SDL_EVENT_WINDOW_RESIZED: { windowWidth  = event.window.data1; windowHeight = event.window.data2; break; }
                default: { break; }
            }
        }
    }

    void Application::BeginFrame() noexcept
    {
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFBO);
        glViewport(0, 0, nativeWidth, nativeHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Application::EndFrame() noexcept
    {
        /** Blit native buffer with integer scaling. */
        glBindFramebuffer(GL_READ_FRAMEBUFFER, nativeFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        const int scale = std::max(1, std::min(windowWidth / nativeWidth, windowHeight / nativeHeight));
        const int blitWidth  = nativeWidth  * scale;
        const int blitHeight = nativeHeight * scale;
        const int xOffset    = (windowWidth  - blitWidth)  / 2;
        const int yOffset    = (windowHeight - blitHeight) / 2;

        glBlitFramebuffer(0, 0, nativeWidth, nativeHeight, xOffset, yOffset, xOffset + nativeWidth, yOffset + nativeHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        SDL_GL_SwapWindow(window);
    }

    void Application::CreateWindow(const Config& config)
    {
        TE_LOG_INFO("Initialising SDL...");

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            TE_LOG_ERROR("SDL_Init failed: {}", SDL_GetError());
            std::abort();
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        const Uint32 flags = SDL_WINDOW_OPENGL | (config.resizable ? SDL_WINDOW_RESIZABLE : 0);

        window = SDL_CreateWindow(config.title.data(), config.windowWidth, config.windowHeight, flags);

        if (!window)
        {
            TE_LOG_ERROR("SDL_CreateWindow failed: {}", SDL_GetError());
            std::abort();
        }

        windowWidth = config.windowWidth;
        windowHeight = config.windowHeight;
    }

    void Application::CreateGLContext()
    {
        glContext = SDL_GL_CreateContext(window);
        if (!glContext)
        {
            TE_LOG_ERROR("SDL_GL_CreateContext failed: {}", SDL_GetError());
            std::abort();
        }

        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)))
        {
            TE_LOG_ERROR("gladLoadGL failed.");
            std::abort();
        }

        SDL_GL_SetSwapInterval(0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        TE_LOG_INFO("OpenGL {}.{} initialised. Vendor: {}", 4, 3, reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    }

    void Application::CreateNativeFramebuffer(int width, int height)
    {
        nativeWidth = width;
        nativeHeight = height;

        glGenTextures(1, &nativeTex);
        glBindTexture(GL_TEXTURE_2D, nativeTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenFramebuffers(1, &nativeFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nativeTex, 0);

        GLuint rboDepth = 0;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            TE_LOG_ERROR("Native Framebuffer Object incomplete.");
            std::abort();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Application::SetupGLDebugCallback()
    {
    #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        TE_LOG_INFO("OpenGL debug callback enabled.");
    #endif
    }

    void Application::Shutdown() noexcept
    {
        if (!window) return;

        TE_LOG_INFO("Shutting down...");

        glDeleteFramebuffers(1, &nativeFBO);
        glDeleteTextures(1, &nativeTex);

        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

        window    = nullptr;
        glContext = nullptr;
        running   = false;

        TE_LOG_INFO("Goodbye!");
    }
}