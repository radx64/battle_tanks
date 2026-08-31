#include "engine/core/MouseCode.hpp"

#include <fmt/format.h>

namespace engine::core
{

std::string toString(const MouseWheel mw)
{
    if (mw == MouseWheel::Horizontal) return "Horizontal";
    else return "Vertical";
}

std::string toString(const MouseButton mb)
{
    switch (mb)
    {
        case MouseButton::Left: return "Left";
        case MouseButton::Right: return "Right";
        case MouseButton::Middle: return "Middle";
        case MouseButton::Extra1: return "Extra1";
        case MouseButton::Extra2: return "Extra2";
    }
    return "Unknown";
}

}  // namespace engine::core
