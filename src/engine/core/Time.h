#ifndef TERRANENGINE_TIME_H
#define TERRANENGINE_TIME_H

#include <chrono>

namespace TerranEngine
{
    /** @brief Static utility for frame timing, delta management, and fixed-step accumulation. */
    class Time
    {
    public:
        // Prevent instantiation or copying.
        Time()                       = delete;
        ~Time()                      = delete;
        Time(const Time&)            = delete;
        Time& operator=(const Time&) = delete;
        Time(Time&&)                 = delete;
        Time& operator=(Time&&)      = delete;


        /** Initialize the timing system. This is called in Application's initialisation step. */
        static void Init() noexcept;

        /** Advance internal clocks and compute deltas each frame */
        static void Tick() noexcept;

        /** The time in seconds since the last tick scaled to the time-scale. */
        [[nodiscard]] static float DeltaTime()         noexcept { return static_cast<float>(deltaTime); }

        /** The raw time in seconds since the last tick. */
        [[nodiscard]] static float DeltaTimeUnscaled() noexcept { return static_cast<float>(deltaTimeRaw); }

        /** Fixed timestep duration (e.g., 1/60s). */
        [[nodiscard]] static float FixedDeltaTime()    noexcept { return static_cast<float>(targetFixedDelta); }

        /** The amount of fixed ticks to consume this frame. */
        [[nodiscard]] static int ConsumeFixedTicks() noexcept;

        /** Total seconds since initialisation. */
        [[nodiscard]] static double TotalTime() noexcept;

        /** Global time-scale controls.Timescale '0.5' is half speed, while '2' is double speed. '0' is total pause. */
        static void SetTimeScale(float scale)  noexcept { timeScale = scale; }
        [[nodiscard]] static float TimeScale() noexcept { return timeScale; }

        /** Smoothed frames-per-second using EMA. */
        [[nodiscard]] static float FPS() noexcept { return smoothedFPS; }

    private:
        /** High-resolution clock alias. */
        using clock = std::chrono::high_resolution_clock;

        inline static clock::time_point startTime;
        inline static clock::time_point lastTime;

        inline static double deltaTimeRaw   = 0.0;
        inline static double deltaTime      = 0.0;
        inline static double accumulator    = 0.0;
        inline static int    fixedTickCount = 0;
        inline static float  timeScale      = 1.0f;

        static constexpr double targetFixedDelta = 1.0 / 60.0;

        inline static double fpsTimer      = 0.0;
        inline static int    fpsFrameCount = 0;
        inline static float smoothedFPS    = 60.0f;
        static constexpr float fpsAlpha    = 0.1f;
    };
}

#endif // TERRANENGINE_TIME_H