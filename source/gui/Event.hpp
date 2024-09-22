#pragma once
// I know sfml have own event subsystem but I'm redefining it 
// to separate from SFML later on

#include <cstdint>

#include <bitset>

#include <engine/Options.hpp>

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

    enum class KeyboardModifier
    {
        Alt = 0,
        Shift,
        Control,
        System,
        Last
    };

    /* Keyboard events */
    struct KeyboardKeyPressed
    {
        Key key;
        engine::Options<KeyboardModifier> modifiers;
    };

    struct KeyboardKeyReleased
    {
        Key key;
        engine::Options<KeyboardModifier> modifiers;
    };

    struct TextEntered
    {
        uint32_t unicode;
    };

}  // namespace gui::event
