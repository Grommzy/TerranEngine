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

    enum class MouseButton : uint8_t
    {
        Left, Right, Middle, X1, X2,

        Count
    };

    struct MouseState
    {
        glm::ivec2 position   {0, 0};
        glm::ivec2 delta      {0, 0};
        glm::ivec2 wheelDelta {0, 0};
        std::bitset<(size_t)MouseButton::Count> current, previous;

        [[nodiscard]] bool IsDown(MouseButton button)      const noexcept { return current.test((size_t)button); }
        [[nodiscard]] bool WasPressed(MouseButton button)  const noexcept { return current.test((size_t)button) && !previous.test((size_t)button); }
        [[nodiscard]] bool WasReleased(MouseButton button) const noexcept { return !current.test((size_t)button) && previous.test((size_t)button); }
    };

    class Input
    {
    public:
        static void OnSDLEvent(const SDL_Event& event);
        static void NewFrame() noexcept;

        [[nodiscard]] static bool IsDown(Key key)       noexcept { return  keyboardCurrent.test((size_t)key); }
        [[nodiscard]] static bool WasPressed(Key key)   noexcept { return  keyboardCurrent.test((size_t)key) && !keyboardPrevious.test((size_t)key); }
        [[nodiscard]] static bool WasReleased(Key key)  noexcept { return !keyboardCurrent.test((size_t)key) && keyboardPrevious.test((size_t)key); }

        [[nodiscard]] static glm::ivec2 MousePosition() noexcept { return mouse.position; }
        [[nodiscard]] static glm::ivec2 MouseDelta()    noexcept { return mouse.delta; }
        [[nodiscard]] static glm::ivec2 WheelDelta()    noexcept { return mouse.wheelDelta; }
        [[nodiscard]] static bool IsMouseDown(MouseButton button)      noexcept { return mouse.IsDown(button); }
        [[nodiscard]] static bool WasMousePressed(MouseButton button)  noexcept { return mouse.WasPressed(button); }
        [[nodiscard]] static bool WasMouseReleased(MouseButton button) noexcept { return mouse.WasReleased(button); }

    private:
        [[nodiscard]] static Key FromSDLScancode(SDL_Scancode scanCode);
        [[nodiscard]] static MouseButton FromSDLButton(uint8_t button);

    private:
        inline static std::bitset<(size_t)Key::Count> keyboardCurrent, keyboardPrevious; // Keyboard
        inline static MouseState mouse;                                  // Mouse
    };
}

#endif // TERRANENGINE_INPUT_H