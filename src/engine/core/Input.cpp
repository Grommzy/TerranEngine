#include "engine/core/Input.h"

#include <array>

namespace TerranEngine
{
    constexpr std::array<Key, SDL_SCANCODE_COUNT> BuildScancodeLUT()
    {
        std::array<Key, SDL_SCANCODE_COUNT> lookupTable {};
        lookupTable.fill(Key::Count);

        lookupTable[SDL_SCANCODE_0] = Key::Num0; lookupTable[SDL_SCANCODE_1] = Key::Num1;
        lookupTable[SDL_SCANCODE_2] = Key::Num2; lookupTable[SDL_SCANCODE_3] = Key::Num3;
        lookupTable[SDL_SCANCODE_4] = Key::Num4; lookupTable[SDL_SCANCODE_5] = Key::Num5;
        lookupTable[SDL_SCANCODE_6] = Key::Num6; lookupTable[SDL_SCANCODE_7] = Key::Num7;
        lookupTable[SDL_SCANCODE_8] = Key::Num8; lookupTable[SDL_SCANCODE_9] = Key::Num9;

        lookupTable[SDL_SCANCODE_A] = Key::A; lookupTable[SDL_SCANCODE_B] = Key::B;
        lookupTable[SDL_SCANCODE_C] = Key::C; lookupTable[SDL_SCANCODE_D] = Key::D;
        lookupTable[SDL_SCANCODE_E] = Key::E; lookupTable[SDL_SCANCODE_F] = Key::F;
        lookupTable[SDL_SCANCODE_G] = Key::G; lookupTable[SDL_SCANCODE_H] = Key::H;
        lookupTable[SDL_SCANCODE_I] = Key::I; lookupTable[SDL_SCANCODE_J] = Key::J;
        lookupTable[SDL_SCANCODE_K] = Key::K; lookupTable[SDL_SCANCODE_L] = Key::L;
        lookupTable[SDL_SCANCODE_M] = Key::M; lookupTable[SDL_SCANCODE_N] = Key::N;
        lookupTable[SDL_SCANCODE_O] = Key::O; lookupTable[SDL_SCANCODE_P] = Key::P;
        lookupTable[SDL_SCANCODE_Q] = Key::Q; lookupTable[SDL_SCANCODE_R] = Key::R;
        lookupTable[SDL_SCANCODE_S] = Key::S; lookupTable[SDL_SCANCODE_T] = Key::T;
        lookupTable[SDL_SCANCODE_U] = Key::U; lookupTable[SDL_SCANCODE_V] = Key::V;
        lookupTable[SDL_SCANCODE_W] = Key::W; lookupTable[SDL_SCANCODE_X] = Key::X;
        lookupTable[SDL_SCANCODE_Y] = Key::Y; lookupTable[SDL_SCANCODE_Z] = Key::Z;

        lookupTable[SDL_SCANCODE_UP]     =  Key::Up   ; lookupTable[SDL_SCANCODE_DOWN] = Key::Down;
        lookupTable[SDL_SCANCODE_RIGHT]  =  Key::Right; lookupTable[SDL_SCANCODE_LEFT] = Key::Left;

        lookupTable[SDL_SCANCODE_SPACE]  = Key::Space ; lookupTable[SDL_SCANCODE_RETURN]    = Key::Enter    ;
        lookupTable[SDL_SCANCODE_LSHIFT] = Key::LShift; lookupTable[SDL_SCANCODE_RSHIFT]    = Key::RShift   ;
        lookupTable[SDL_SCANCODE_LALT]   = Key::LAlt  ; lookupTable[SDL_SCANCODE_RALT]      = Key::RAlt     ;
        lookupTable[SDL_SCANCODE_LCTRL]  = Key::LCtrl ; lookupTable[SDL_SCANCODE_RCTRL]     = Key::RCtrl    ;
        lookupTable[SDL_SCANCODE_TAB]    = Key::Tab   ; lookupTable[SDL_SCANCODE_BACKSPACE] = Key::Backspace;
        lookupTable[SDL_SCANCODE_ESCAPE] = Key::Escape;

        return lookupTable;
    }

    static constexpr std::array<Key, SDL_SCANCODE_COUNT> scancodeLUT = BuildScancodeLUT();

    inline Key Input::FromSDLScancode(SDL_Scancode scancode)
    {
        return scancode < scancodeLUT.size() ? scancodeLUT[scancode] : Key::Count;
    }

    inline MouseButton Input::FromSDLButton(uint8_t button)
    {
        switch (button)
        {
            case SDL_BUTTON_LEFT:   { return MouseButton::Left;   }
            case SDL_BUTTON_RIGHT:  { return MouseButton::Right;  }
            case SDL_BUTTON_MIDDLE: { return MouseButton::Middle; }
            case SDL_BUTTON_X1:     { return MouseButton::X1;     }
            case SDL_BUTTON_X2:     { return MouseButton::X2;     }

            default:                { return MouseButton::Count;  }
        }
    }

    void Input::OnSDLEvent(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_EVENT_KEY_DOWN:
            {
                Key key = FromSDLScancode(event.key.scancode);
                if (key != Key::Count) { keyboardCurrent.set((size_t)key, true); }
                break;
            }

            case SDL_EVENT_KEY_UP:
            {
                Key key = FromSDLScancode(event.key.scancode);
                if (key != Key::Count) { keyboardCurrent.set((size_t)key, false); }
                break;
            }

            case SDL_EVENT_MOUSE_MOTION:
            {
                mouse.position = {event.motion.x   , event.motion.y};
                mouse.delta    = {event.motion.xrel, event.motion.yrel};
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseButton button = FromSDLButton(event.button.button);
                if (button != MouseButton::Count) { mouse.current.set((size_t)button, true); }
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButton button = FromSDLButton(event.button.button);
                if (button != MouseButton::Count) { mouse.current.set((size_t)button, false); }
                break;
            }

            case SDL_EVENT_MOUSE_WHEEL:
            {
                mouse.wheelDelta += glm::ivec2 {event.wheel.x, event.wheel.y};
                break;
            }

            default: { break; }
        }
    }

    void Input::NewFrame() noexcept
    {
        keyboardPrevious = keyboardCurrent;
        mouse.previous   = mouse.current;
        mouse.delta      = {0, 0};
        mouse.wheelDelta = {0, 0};
    }
}