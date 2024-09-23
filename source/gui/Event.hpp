#pragma once
// I know sfml have own event subsystem but I'm redefining it 
// to separate from SFML later on

#include <cstdint>

#include <bitset>

#include <SFML/Window/Keyboard.hpp>

namespace gui::event
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

    using Key = sf::Keyboard::Key;

    struct KeyModifiers
    {
        bool alt;
        bool shift;
        bool control;
        bool system;
    };

    /* Keyboard events */
    struct KeyboardKeyPressed
    {
        Key key;
        KeyModifiers modifiers;
    };

    struct KeyboardKeyReleased
    {
        Key key;
        KeyModifiers modifiers;
    };

    struct TextEntered
    {
        uint32_t unicode;
    };

}  // namespace gui::event
