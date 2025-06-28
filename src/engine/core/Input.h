#ifndef TERRANENGINE_INPUT_H
#define TERRANENGINE_INPUT_H

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include <cstdint>
#include <bitset>

namespace TerranEngine
{
    enum class Key : uint8_t
    {
        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,

        A, B, C, D, E, F, G, H, I, J,
        K, L, M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z,

        Left, Right, Up, Down,
        Space, Enter, Escape,
        LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,
        Tab, Backspace, CapsLock,

        Count
    };

    class Input
    {
    public:
        static void OnSDLEvent(const SDL_Event& event);
        static void NewFrame() noexcept;

        static bool IsDown(Key key)       noexcept { return  current.test((size_t)key); }
        static bool WasPressed(Key key)   noexcept { return  current.test((size_t)key) && !previous.test((size_t)key); }
        static bool WasReleased(Key key)  noexcept { return !current.test((size_t)key) && previous.test((size_t)key); }

        static glm::ivec2 MousePosition() noexcept { return mouse; }
        static glm::ivec2 MouseDelta()    noexcept { return mouseDelta; }

    private:
        [[nodiscard]] static Key FromSDLScancode(SDL_Scancode scanCode);

    private:
        inline static std::bitset<(size_t)Key::Count> current, previous;
        inline static glm::ivec2 mouse      {0, 0};
        inline static glm::ivec2 mouseDelta {0, 0};
    };
}

#endif // TERRANENGINE_INPUT_H