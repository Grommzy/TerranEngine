#include "engine/core/Time.h"
#include "engine/core/Log.h"

namespace TerranEngine
{
    // --- Helper ------------------------------------
    double Time::NowSeconds() const noexcept
    {
        return static_cast<double>(SDL_GetTicks() / 1000.0);
    }

    // --- Constructor -------------------------------
    Time::Time()
    {
        prevTime = NowSeconds();
    }

    // --- Public Interface --------------------------
    void Time::StartFrame()
    {
        const double current = NowSeconds();
        double delta = (current - prevTime) * timeScale;

        // Clamp values to avoid spiral-of-death on long hitches.
        if (delta > maxFrameDelta)
        {
            delta = maxFrameDelta;
        }

        prevTime = current;
        frameDelta = delta;
        totalTime += delta;
        accumulator += delta;

        // FPS EMA.
        if (frameCounter == 0)
        {
            fpsEMA = 1.0 / frameDelta;
        }
        else
        {
            const double instFPS = 1.0 / frameDelta;
            fpsEMA = fpsSmoothing * fpsEMA + (1.0 - fpsSmoothing) * instFPS;
        }

        ++frameCounter;
    }

    bool Time::StepFixed()
    {
        if (accumulator >= fixedStep)
        {
            accumulator -= fixedStep;
            alpha = accumulator / fixedStep;
            return true;
        }

        return false;
    }

    void Time::SetTimeScale(float scale) noexcept
    {
        timeScale = scale < 0.0 ? 0.0 : scale;
    }

    void Time::Reset() noexcept
    {
        prevTime = NowSeconds();
        accumulator = 0.0;
        frameDelta = 0.0;
        totalTime = 0.0;
        frameCounter = 0;
        fpsEMA = 0.0f;
        alpha = 0.0;
    }
}