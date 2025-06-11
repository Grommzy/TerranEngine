#include "engine/core/Application.h"

#include "engine/core/Log.h"

#include <algorithm>    // std::max
#include <cstdlib>      //std::abort
#include <cstring>      // std::strlen

namespace
{
    constexpr GLenum FramebufferTarget = GL_FRAMEBUFFER;

    #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
        void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user)
        {
            using namespace TerranEngine;

            const char* severityString =
                (severity == GL_DEBUG_SEVERITY_HIGH)   ? "HIGH"   :
                (severity == GL_DEBUG_SEVERITY_MEDIUM) ? "MEDIUM" :
                (severity == GL_DEBUG_SEVERITY_LOW)    ? "LOW"    :
                                                         "NOTIFY" ;

            switch(severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:   TE_LOG_ERROR("[GL {}] {}", severityString, message); break;
                case GL_DEBUG_SEVERITY_MEDIUM: TE_LOG_WARN ("[GL {}] {}", severityString, message); break;
                default:                       TE_LOG_INFO ("[GL {}] {}", severityString, message); break;
            }
        }
    #endif
} // Anonymous namespace.

namespace TerranEngine
{
    /*********************************************** Constuction / Destruction **********************************************/

    Application::Application(const Config& config)
    {
        TE_LOG_INFO("Initialising SDL...");
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            TE_LOG_ERROR("SDL_Init failed: {}", SDL_GetError());
            std::abort();
        }

        CreateWindow(config);
        CreateGLContext();
        CreateNativeFramebuffer(config.nativeWidth, config.nativeHeight);

        #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
            SetupGLDebugCallback();
        #endif

        prevTicks = SDL_GetTicks();
        TE_LOG_INFO("Application initialisation complete.");
    }

    Application::~Application()
    {
        TE_LOG_INFO("Shutting down...");

        if (nativeTexture) glDeleteTextures(1, &nativeTexture);
        if (nativeFBO)     glDeleteFramebuffers(1, &nativeFBO);
        if (glContext)     SDL_GL_DestroyContext(glContext);
        if (window)        SDL_DestroyWindow(window);

        SDL_Quit();
    }

    /*********************************************** Public Interface **********************************************/

    bool Application::PumpEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT: { return false; break; }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    break;
                }
            }
        }

        const uint64_t now = SDL_GetTicks();
        deltaTime          = static_cast<double>(now - prevTicks) / 1000.0;
        prevTicks          = now;

        return true;
    }

    void Application::BeginFrame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFBO);
        glViewport(0, 0, nativeWidth, nativeHeight);
    }

    void Application::EndFrame()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, nativeFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        const int scale = std::max(1, std::min(windowWidth / nativeWidth, windowHeight / nativeHeight));

        const int blitWidth  = nativeWidth  * scale;
        const int blitHeight = nativeHeight * scale;
        const int xOffset = (windowWidth  - blitWidth)  / 2;
        const int yOffset = (windowHeight - blitHeight) / 2;

        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        glBlitFramebuffer(0, 0, nativeWidth, nativeHeight, xOffset, yOffset, xOffset + blitWidth, yOffset + blitHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        SDL_GL_SwapWindow(window);
    }

    /*********************************************** Private Helpers **********************************************/

    void Application::CreateWindow(const Config& config)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,                           4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,                           3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,                                    1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,                                      0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,                                    0);

        const Uint32 flags = SDL_WINDOW_OPENGL | (config.resizable ? SDL_WINDOW_RESIZABLE : 0);

        window = SDL_CreateWindow(config.title.data(), config.windowWidth, config.windowHeight, flags);

        if (!window)
        {
            TE_LOG_ERROR("SDL_CreateWindow failed: {}", SDL_GetError());
            std::abort();
        }

        windowWidth  = config.windowWidth;
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

        TE_LOG_INFO("OpenGL {}.{} initialised. Vendor: {}", 4, 3, reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    }

    void Application::CreateNativeFramebuffer(int width, int height)
    {
        nativeWidth = width;
        nativeHeight = height;

        glGenTextures(1, &nativeTexture);
        glBindTexture(GL_TEXTURE_2D, nativeTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenFramebuffers(1, &nativeFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nativeTexture, 0);

        if (glCheckFramebufferStatus(FramebufferTarget) != GL_FRAMEBUFFER_COMPLETE)
        {
            TE_LOG_ERROR("Native framebuffer is incomplete!");
            std::abort();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Application::SetupGLDebugCallback()
    {
        #if TE_ENABLE_GL_DEBUG
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
            TE_LOG_INFO("OpenGL debug callback enabled.");
        #else
            TE_LOG_INFO("OpenGL debug callback not enabled (TE_ENABLE_GL_DEBUG=0).");
        #endif
    }
} // TerranEngine namespace.