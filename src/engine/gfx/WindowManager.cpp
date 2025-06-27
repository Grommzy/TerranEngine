#include "engine/gfx/WindowManager.h"

#include "engine/core/Log.h"

#include <cassert>
#include <algorithm>

namespace TerranEngine
{
    bool WindowManager::Initialise(const Config& config)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            TE_LOG_ERROR("SDL_Init failed: {}", SDL_GetError());
            return false;
        }

        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        #endif

        // Create window.
        Uint32 windowFlags = SDL_WINDOW_OPENGL | (config.resizable ? SDL_WINDOW_RESIZABLE : 0);

        window = SDL_CreateWindow(config.title.c_str(), config.windowWidth, config.windowHeight, windowFlags);
        if (!window)
        {
            TE_LOG_ERROR("SDL_CreateWindow failed: {}", SDL_GetError());
            return false;
        }

        windowWidth = config.windowWidth;
        windowHeight = config.windowHeight;

        // Create OpenGL Context.
        glContext = SDL_GL_CreateContext(window);
        if (!glContext)
        {
            TE_LOG_ERROR("SDL_GL_CreateContext failed: {}", SDL_GetError());
            return false;
        }

        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
        {
            TE_LOG_ERROR("Failed to initialise GLAD.");
            return false;
        }

        vsync = config.vsync;
        SDL_GL_SetSwapInterval(vsync ? 1 : 0);

        TE_LOG_INFO("OpenGL {}.{} | Vendor: {} | Renderer: {}", 4, 3, reinterpret_cast<const char*>(glGetString(GL_VENDOR)), reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

        #if defined(TE_ENABLE_GL_DEBUG) && TE_ENABLE_GL_DEBUG
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(GlDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        #endif

        // Framebuffer for pixel-perfection and window-blitting.
        targetWidth  = nativeWidth  = config.nativeWidth;
        targetHeight = nativeHeight = config.nativeHeight;
        growViewport = config.growViewport;
        CreateNativeFramebuffer();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        return true;
    }

    void WindowManager::BeginFrame()
    {
        // Integer scale + axis growth.
        scale = std::max(1, std::min(windowWidth / targetWidth, windowHeight / targetHeight));

        const int baseWidth  = targetWidth  * scale;
        const int baseHeight = targetHeight * scale;

        const int extraWidth  = (windowWidth  - baseWidth)  / scale;
        const int extraHeight = (windowHeight - baseHeight) / scale;

        // Grow the axis with the larger leftover gap.
        viewportWidth  = targetWidth  + (extraWidth >= extraHeight ? extraWidth  : 0);
        viewportHeight = targetHeight + (extraHeight > extraWidth  ? extraHeight : 0);

        if (!growViewport)
        {
            viewportWidth  = targetWidth;
            viewportHeight = targetHeight;
        }

        // Resize FBO if the viewport was allowed to grow.
        if (growViewport && (viewportWidth != nativeWidth || viewportHeight != nativeHeight))
        {
            nativeWidth  = viewportWidth;
            nativeHeight = viewportHeight;

            DestroyNativeFramebuffer();
            CreateNativeFramebuffer();
        }

        xOffset = (windowWidth  - nativeWidth  * scale)  / 2;
        yOffset = (windowHeight - nativeHeight * scale)  / 2;

        glBindFramebuffer(GL_FRAMEBUFFER, nativeFBO);
        glViewport(0, 0, nativeWidth, nativeHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void WindowManager::EndFrame()
    {
        // Blit native rendering size to the window rendering size using nearest filtering.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, nativeFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        const int blitWidth  = nativeWidth  * scale;
        const int blitHeight = nativeHeight * scale;

        glBlitFramebuffer(0, 0, nativeWidth, nativeHeight, xOffset, yOffset, xOffset + blitWidth, yOffset + blitHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        SDL_GL_SwapWindow(window);
    }

    void WindowManager::Shutdown()
    {
        DestroyNativeFramebuffer();

        if (glContext)
        {
            SDL_GL_DestroyContext(glContext);
            glContext = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }

    void WindowManager::CreateNativeFramebuffer()
    {
        // Colour texture.
        glCreateTextures(GL_TEXTURE_2D, 1, &nativeColour);
        glTextureStorage2D(nativeColour, 1, GL_RGBA8, nativeWidth, nativeHeight);
        glTextureParameteri(nativeColour, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(nativeColour, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Depth renderbuffer.
        glCreateRenderbuffers(1, &nativeDepth);
        glNamedRenderbufferStorage(nativeDepth, GL_DEPTH_COMPONENT24, nativeWidth, nativeHeight);

        // Framebuffer Object (FBO)
        glCreateFramebuffers(1, &nativeFBO);
        glNamedFramebufferTexture(nativeFBO, GL_COLOR_ATTACHMENT0, nativeColour, 0);
        glNamedFramebufferRenderbuffer(nativeFBO, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, nativeDepth);

        if (glCheckNamedFramebufferStatus(nativeFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            TE_LOG_ERROR("Native framebuffer incomplete.");
            std::abort();
        }
    }

    void WindowManager::DestroyNativeFramebuffer() noexcept
    {
        glDeleteFramebuffers(1, &nativeFBO);
        glDeleteTextures(1, &nativeColour);
        glDeleteRenderbuffers(1, &nativeDepth);
        nativeFBO    = 0;
        nativeColour = 0;
        nativeDepth  = 0;
    }

    void GLAPIENTRY WindowManager::GlDebugCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) { return; }
        const char* typeCString = (type == GL_DEBUG_TYPE_ERROR) ? "ERROR" : "MESSAGE";
        TE_LOG_ERROR("[OpenGL {}] {}", typeCString, message);
    }
}