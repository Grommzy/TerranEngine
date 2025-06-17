#ifndef TIME_H
#define TIME_H

#include <SDL3/SDL.h>
#include <cstdint>

namespace TerranEngine
{
    class Time
    {
    public:
        Time();

        // --- Per-Frame Control ----------------------------------------------
        void StartFrame();
        bool StepFixed();   // Returns true while there's pending fixed tick.

        [[nodiscard]] double   DeltaTime()  const noexcept { return frameDelta;   }
        [[nodiscard]] double   Alpha()      const noexcept { return alpha;        }
        [[nodiscard]] double   Total()      const noexcept { return totalTime;    }
        [[nodiscard]] uint64_t FrameCount() const noexcept { return frameCounter; }
        [[nodiscard]] float    FPS()        const noexcept { return fpsEMA;       }
        [[nodiscard]] double   FixedStep()  const noexcept { return fixedStep;    }

        void SetTimeScale(float scale) noexcept; // 0 = pause, 1 = real-time.
        void Reset() noexcept;               // Zero counters.

    private:
        double NowSeconds() const noexcept;
    
    private:
        // --- Config ----------------------------------------------
        double fixedStep     { 1.0 / 60.0 }; // logic tick (seconds).
        double maxFrameDelta { 0.25 };       // Frame-hitch clamp.

        double prevTime    { 0.0 };
        double accumulator { 0.0 };
        double frameDelta  { 0.0 };
        double alpha       { 0.0 };
        float  timeScale   { 1.0f };

        double totalTime      { 0.0 };
        uint64_t frameCounter { 0 };

        // --- FPS smoothing (Exponential Moving Average) ----------------------------------------------
        float fpsEMA { 0.0f };
        const float fpsSmoothing = 0.9f; // 0 = instant, 0.9 = 60-frame average.
    };
}

#endif // TIME_H