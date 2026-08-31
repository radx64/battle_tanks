#pragma once
// I know sfml have own event subsystem but I'm redefining it
// to separate from SFML later on

#include <cstdint>

#include <SFML/Window/Keyboard.hpp>

namespace engine::gui::event
{
    /* Mouse events */
    struct MousePosition
    {
        float x;
        float y;
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,
    };

    struct MouseMoved
    {
        MousePosition position;
    };

    struct MouseButtonPressed
    {
        MouseButton button;
        MousePosition position;
    };

    struct MouseButtonDoublePressed
    {
        MouseButton button;
        MousePosition position;
    };

    struct MouseButtonReleased
    {
        MouseButton button;
        MousePosition position;
    };

    struct MouseEntered
    {
    };

    struct MouseLeft
    {
    };

    struct FocusChange
    {
        enum class Type
        {
            Next,
            Previous
        };

        Type type;
    };

    struct FocusGained
    {
    };

    struct FocusLost
    {
    };

}  // namespace engine::gui::event
