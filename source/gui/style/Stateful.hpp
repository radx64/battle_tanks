#pragma once

#include "gui/WidgetState.hpp"

namespace gui::style
{

template <typename T>
struct Stateful
{
    T normal {};
    T hovered {};
    T pressed {};

    const T& get(WidgetState state) const
    {
        if (state.pressed) return pressed;
        if (state.hovered) return hovered;
        return normal;
    }

    static Stateful<T> sameInAllStates(const T& value)
    {
        return Stateful<T>{.normal = value, .hovered = value, .pressed = value};
    }
};

}  // gui::style
