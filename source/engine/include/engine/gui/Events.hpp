#pragma once

#include <cstdint>

#include "engine/events/MouseEvent.hpp"

// This file extends events types for gui specific events. It is not meant to be used outside of gui module

namespace engine::gui::events
{
    struct MouseEntered
    {
    };

    struct MouseLeft
    {
    };

    struct MouseButtonDoublePressed : public engine::events::MouseButtonPressed
    {
        MouseButtonDoublePressed(const engine::Vec2f& position, const engine::core::MouseButton button)
        : engine::events::MouseButtonPressed(position, button)
        {}
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

}  // namespace engine::gui::events
