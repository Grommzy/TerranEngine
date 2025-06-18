#include "engine/core/Time.h"

namespace TerranEngine
{
    using clock = std::chrono::high_resolution_clock;

    void Time::Init() noexcept
    {
        startTime = clock::now();
        lastTime  = startTime;
    }

    void Time::Tick() noexcept
    {
        clock::time_point now = clock::now();
        std::chrono::duration<double> difference = now - lastTime;
        lastTime = now;

        // Apply time-scale to time-delta.
        deltaTimeRaw = difference.count();
        deltaTime    = difference.count() * timeScale;

        ++fpsFrameCount;
        fpsTimer += deltaTimeRaw;
        // Update the FPS timer every half-second.
        if (fpsTimer >= 0.5)
        {
            const float currentFPS = static_cast<float>(fpsFrameCount / fpsTimer);
            smoothedFPS = smoothedFPS * (1.0f - fpsAlpha) + currentFPS * fpsAlpha;
            fpsFrameCount = 0;
            fpsTimer      = 0.0;
        }

        // Accumulate timesteps for fixed-step logic.
        accumulator += difference.count();
        fixedTickCount = 0;

        // Check for how many fixed-steps took place within the tick.
        while (accumulator >= targetFixedDelta)
        {
            accumulator -= targetFixedDelta;
            ++fixedTickCount;
        }
    }

    int Time::ConsumeFixedTicks() noexcept
    {
        int count = fixedTickCount;
        fixedTickCount = 0;

        return count;
    }

    double Time::TotalTime() noexcept
    {
        std::chrono::duration<double> difference = lastTime - startTime;

        return difference.count();
    }

}